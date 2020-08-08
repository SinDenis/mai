//
// Created by Sin Denis on 2019-11-25.
//

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <fstream>
#include <cstdint>
#include <memory>
#include <string>
#include <algorithm>
#include "Figures.h"
#include <list>

const uint32_t MAGIC_NUMBER = 2012000;

class Document {
public:
	Document(): Id_(1), Name_(""), Buf_(0), rhombusFactory(), pentagonFactory(), hexagonFactory()
	{
	}

	explicit Document(std::string name):
	Id_(1),
	Name_(std::move(name)),
	Buf_(0),
	rhombusFactory(),
	pentagonFactory(),
	hexagonFactory()
	{
	}

	~Document() = default;

	void Rename(const std::string& newName) {
		Name_ = newName;
	}

	void Save(const std::string& filename) const {
		SerializeImpl(filename);
	}

	void SaveJson(const std::string& filename) const {
		SerializeJsonImpl(filename);
	}

	void Load(const std::string& filename) {
		DeserializeImpl(filename);
	}

	void Print() {
		std::for_each(Buf_.begin(), Buf_.end(), [](std::shared_ptr<IFigure> shape) {
			shape->Print(std::cout) << "\n";
		});
	}

	void RemovePrimitive(uint32_t id) {
		auto it = std::find_if(Buf_.begin(), Buf_.end(), [id](std::shared_ptr<IFigure> shape) -> bool {
			return id == shape->Id();
		});

		if (it == Buf_.end())
			throw std::logic_error("Figure with this id doesn't exist");

		Buf_.erase(it);
	}

	void InsertPrimitive(FigureType type, const std::pair<double, double>& center, const std::pair<double, double>& point) {
		switch (type) {
			case RHOMBUS:
				Buf_.push_back(rhombusFactory.FigureCreate(Id_++, center, point));
				break;
			case PENTAGON:
				Buf_.push_back(pentagonFactory.FigureCreate(Id_++, center, point));
				break;
			case HEXAGON:
				Buf_.push_back(hexagonFactory.FigureCreate(Id_++, center, point));
				break;
		}
	}

private:
	uint32_t Id_;
	std::string Name_;
	std::list<std::shared_ptr<IFigure>> Buf_;
	RhombusFactory rhombusFactory;
	PentagonFactory pentagonFactory;
	HexagonFactory hexagonFactory;

	friend class InsertCommand;
	friend class RemoveCommand;

	void SerializeImpl(const std::string& filename) const {
		std::ofstream os;

		os.open(filename, std::ios_base::binary | std::ios_base::out);
		if (!os.is_open()) {
			throw std::runtime_error("File is not opened");
		}
		uint32_t magicNumber = MAGIC_NUMBER;
		uint32_t nameLen = Name_.size();
		os.write((char*)&magicNumber, sizeof(magicNumber));
		os.write((char*)&nameLen, sizeof(nameLen));
		os.write((char*)(Name_.c_str()), nameLen);
		for (const auto& shape : Buf_) {
			shape->Serialize(os);
		}
	}

	void SerializeJsonImpl(const std::string& filename) const {
		std::ofstream os;
		os.open(filename, std::ios_base::out);
		if (!os.is_open()) {
			throw std::runtime_error("File is not opened");
		}
		os << "[";
		for (auto it = Buf_.cbegin(); it != Buf_.cend();) {
			(*it)->SerializeJson(os);
			auto tmp = it;
			++tmp;
			if (tmp != Buf_.cend())
				os << ",";
			std::advance(it, 1);
		}
		os << "]";
	}

	void DeserializeImpl(const std::string& filename) {
		std::ifstream is;

		is.open(filename, std::ios_base::binary | std::ios_base::in);
		if (!is.is_open()) {
			throw std::runtime_error("File is not opened");
		}
		uint32_t magicNumber;
		uint32_t nameLen;
		is.read((char*)&magicNumber, sizeof(magicNumber));
		if (magicNumber != MAGIC_NUMBER)
			throw std::runtime_error("Bad file");
		is.read((char*)&nameLen, sizeof(nameLen));
		char* name = new char[nameLen + 1];
		name[nameLen] = 0;
		is.read(name, nameLen);
		Name_ = std::string(name);
		delete[] name;
		FigureType type;
		while(true) {
			is.read((char*)&type, sizeof(type));
			if (is.eof())
				break;
			switch (type) {
				case RHOMBUS:
					Buf_.push_back(rhombusFactory.FigureCreate());
					break;
				case PENTAGON:
					Buf_.push_back(pentagonFactory.FigureCreate());
					break;
				case HEXAGON:
					Buf_.push_back(hexagonFactory.FigureCreate());
					break;
			}
			Buf_.back()->Deserialize(is);
		}
		Id_ = Buf_.size();
	}

	std::shared_ptr<IFigure> GetFigure(uint32_t id) {
		auto it = std::find_if(Buf_.begin(), Buf_.end(), [id](std::shared_ptr<IFigure> shape) -> bool {
			return id == shape->Id();
		});
		return *it;
	}

	uint32_t GetPos(uint32_t id) {
		auto it = std::find_if(Buf_.begin(), Buf_.end(), [id](std::shared_ptr<IFigure> shape) -> bool {
			return id == shape->Id();
		});
		return std::distance(Buf_.begin(), it);
	}

	void InsertPrimitive(uint32_t pos, std::shared_ptr<IFigure> figure) {
		auto it = Buf_.begin();
		std::advance(it, pos);
		Buf_.insert(it, figure);
	}

	void RemoveLastPrimitive() {
		if (Buf_.empty())
			throw std::logic_error("Document is empty");
		Buf_.pop_back();
	}
};

#endif
