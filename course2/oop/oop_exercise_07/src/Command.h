//
// Created by Sin Denis on 2019-11-25.
//

#ifndef COMMAND_H
#define COMMAND_H

#include "Document.h"
#include <stack>

class ICommand {
protected:
	std::shared_ptr<Document> Doc;
public:
	virtual ~ICommand() = default;
	virtual void Execute() = 0;
	virtual void UnExecute() = 0;

	void SetDocument(std::shared_ptr<Document> doc) {
		Doc = doc;
	}
};

class InsertCommand : public ICommand {
public:
	InsertCommand(FigureType type, std::pair<double, double> center, std::pair<double, double> point):
		Type_(type), Center_(std::move(center)), Point_(std::move(point))
	{
	}

	void Execute() override {
		Doc->InsertPrimitive(Type_, Center_, Point_);
	}

	void UnExecute() override {
		Doc->RemoveLastPrimitive();
	}

private:
	FigureType Type_;
	std::pair<double, double> Center_;
	std::pair<double, double> Point_;
};

class RemoveCommand : public ICommand {
public:
	explicit RemoveCommand(uint32_t id): Id_(id), Pos_(0), Figure_(nullptr)
	{
	}

	void Execute() override {
		Figure_ = Doc->GetFigure(Id_);
		Pos_ = Doc->GetPos(Id_);
		Doc->RemovePrimitive(Id_);
	}

	void UnExecute() override {
		Doc->InsertPrimitive(Pos_, Figure_);
	}

private:
	uint32_t Id_;
	uint32_t Pos_;
	std::shared_ptr<IFigure> Figure_;
};

#endif
