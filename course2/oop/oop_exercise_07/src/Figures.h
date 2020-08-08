//
// Created by Sin Denis on 2019-11-24.
//

#ifndef FIGURES_H
#define FIGURES_H

#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <cstdint>

static double GetEps()
{
	double epsilon = 1.0;

	while (1.0 + 0.5 * epsilon != 1.0) {
		epsilon *= 0.5;
	}
	return epsilon;
}

enum FigureType {
	RHOMBUS,
	PENTAGON,
	HEXAGON
};

namespace NGeomAlgo {
	double DotProduct(const std::pair<double, double>& v1, const std::pair<double, double>& v2) {
		return v1.first *  v2.first + v1.second * v2.second;
	}

	double DotDistance(const std::pair<double, double>& p1, const std::pair<double, double>& p2) {
		return sqrt(pow((p1.first - p2.first), 2) + pow((p1.second - p2.second), 2));
	}

	int GetQuarter(std::pair<double, double> center, std::pair<double, double> point) {
		if (fabs(point.first) < GetEps() || fabs(point.second) < GetEps())
			return 0;
		if (point.first > center.first && point.second > center.second)
			return 1;
		if (point.first < center.first && point.second > center.second)
			return 2;
		if (point.first < center.first && point.second < center.second)
			return 3;
		if (point.first > center.first && point.second < center.second)
			return 4;
		return 0;
	}

	double CornerCalc(const std::pair<double, double>& v1, const std::pair<double, double>& v2) {
		double radius = DotDistance(v1, v2);
		std::pair<double, double> u1 = std::make_pair(v2.first - v1.first, v2.second - v1.second);
		std::pair<double, double> u2 = std::make_pair(radius, 0);
		double corner = acos(NGeomAlgo::DotProduct(u1, u2) / (radius * radius));
		double quarter = NGeomAlgo::GetQuarter(v1, v2);
		if (quarter == 2)
			return M_PI - corner;
		if (quarter == 3)
			return M_PI + corner;
		return corner;
	}

	double NGonSquare(int n, double radius) {
		return (double)n / 2 * pow(radius, 2) * sin(2 * M_PI / (double)n);
	}

	std::vector<std::pair<double, double>> GetNGonCoors
	(int n, const std::pair<double, double>& center, const std::pair<double, double>& point) {
		std::vector<std::pair<double, double>> res;
		double corner = NGeomAlgo::CornerCalc(center, point);
		double r = NGeomAlgo::DotDistance(center, point);
		res.push_back(point);
		for (int i = 1; i < n; ++i)
			res.emplace_back(center.first + r * cos(corner + 2 * M_PI * i / n), center.second + r * sin(corner + 2 * M_PI * i / n));
		return res;
	}
}

std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& v) {
	os << "(" << v.first << ", " << v.second << ")";
	return os;
}

class IFigure {
public:
	virtual double Square() const = 0;
	virtual std::pair<double, double> Center() const = 0;
	virtual std::ostream& Print(std::ostream& out) const = 0;
	virtual void SerializeJson(std::ofstream& out) const = 0;
	virtual void Serialize(std::ofstream& os) const = 0;
	virtual void Deserialize(std::ifstream& is) = 0;
	virtual uint32_t Id() const = 0;
	virtual ~IFigure() = default;
};

class Rhombus : public IFigure {
public:
	Rhombus(): Id_(0), Center_(std::make_pair(0, 0)), FirstPoint_(std::make_pair(0, 0))
	{
	}

	Rhombus(uint32_t id, std::pair<double, double> center, std::pair<double, double> v1):
		Id_(id), Center_(std::move(center)), FirstPoint_(std::move(v1))
	{
	}

	uint32_t Id() const override {
		return Id_;
	}

	double Square() const override {
		return NGeomAlgo::NGonSquare(4, GetRadius());
	}

	std::pair<double, double> Center() const override {
		return Center_;
	}

	std::ostream& Print(std::ostream& out) const override {
		out << "ID: " << Id_ << "\n";
		out << "Figure: Rhombus" << "\n";
		out << "Coors:\n";
		std::vector<std::pair<double, double>> coors = NGeomAlgo::GetNGonCoors(4, Center_, FirstPoint_);
		for (auto& p : coors)
			out << p << "\n";
		return out;
	}

	void SerializeJson(std::ofstream& out) const override {
		out << "{\"id\": " << Id_ << ",";
		out << "\"figure\": \"Rhombus\",";
		out << "\"coors\": [";
		std::vector<std::pair<double, double>> coors = NGeomAlgo::GetNGonCoors(4, Center_, FirstPoint_);
		for (int i = 0; i < 4; ++i) {
			out << "[" << coors[i].first << ", " << coors[i].second << "]" << "\n";
			if (i != 3)
				out << ",";
		}
		out << "]}";
	}

	void Serialize(std::ofstream& os) const override {
		FigureType type = RHOMBUS;
		os.write(reinterpret_cast<char*>(&type), sizeof(type));
		os.write((char*)(&Id_), sizeof(Id_));
		os.write((char*)(&Center_.first), sizeof(Center_.first));
		os.write((char*)(&Center_.second), sizeof(Center_.second));
		os.write((char*)(&FirstPoint_.first), sizeof(FirstPoint_.first));
		os.write((char*)(&FirstPoint_.second), sizeof(FirstPoint_.second));
	}

	void Deserialize(std::ifstream& is) override {
		is.read((char*)(&Id_), sizeof(Id_));
		is.read((char*)(&Center_.first), sizeof(Center_.first));
		is.read((char*)(&Center_.second), sizeof(Center_.second));
		is.read((char*)(&FirstPoint_.first), sizeof(FirstPoint_.first));
		is.read((char*)(&FirstPoint_.second), sizeof(FirstPoint_.second));
	}

private:
	uint32_t Id_;
	std::pair<double, double> Center_;
	std::pair<double, double> FirstPoint_;

	double GetRadius() const {
		return NGeomAlgo::DotDistance(Center_, FirstPoint_);
	}
};

class Pentagon : public IFigure {
public:
	Pentagon(): Id_(0), Center_(std::make_pair(0, 0)), FirstPoint_(std::make_pair(0, 0))
	{
	}

	uint32_t Id() const override {
		return Id_;
	}

	Pentagon(uint32_t id, std::pair<double, double> center, std::pair<double, double> v1):
		Id_(id), Center_(std::move(center)), FirstPoint_(std::move(v1))
	{
	}

	double Square() const override {
		return NGeomAlgo::NGonSquare(5, GetRadius());
	}

	std::pair<double, double> Center() const override {
		return Center_;
	}

	std::ostream& Print(std::ostream& out) const override {
		out << "ID: " << Id_ << "\n";
		out << "Figure: Pentagon" << "\n";
		out << "Coors:\n";
		std::vector<std::pair<double, double>> coors = NGeomAlgo::GetNGonCoors(5, Center_, FirstPoint_);
		for (auto& p : coors)
			out << p << "\n";
		return out;
	}

	void SerializeJson(std::ofstream& out) const override {
		out << "{\"id\": " << Id_ << ",";
		out << "\"figure\": \"Pentagon\",";
		out << "\"coors\": [";
		std::vector<std::pair<double, double>> coors = NGeomAlgo::GetNGonCoors(5, Center_, FirstPoint_);
		for (int i = 0; i < 5; ++i) {
			out << "[" << coors[i].first << ", " << coors[i].second << "]" << "\n";
			if (i != 4)
				out << ",";
		}
		out << "]}";
	}

	void Serialize(std::ofstream& os) const override {
		FigureType type = PENTAGON;
		os.write(reinterpret_cast<char*>(&type), sizeof(type));
		os.write((char*)(&Id_), sizeof(Id_));
		os.write((char*)(&Center_.first), sizeof(Center_.first));
		os.write((char*)(&Center_.second), sizeof(Center_.second));
		os.write((char*)(&FirstPoint_.first), sizeof(FirstPoint_.first));
		os.write((char*)(&FirstPoint_.second), sizeof(FirstPoint_.second));
	}

	void Deserialize(std::ifstream& is) override {
		is.read((char*)(&Id_), sizeof(Id_));
		is.read((char*)(&Center_.first), sizeof(Center_.first));
		is.read((char*)(&Center_.second), sizeof(Center_.second));
		is.read((char*)(&FirstPoint_.first), sizeof(FirstPoint_.first));
		is.read((char*)(&FirstPoint_.second), sizeof(FirstPoint_.second));
	}

private:
	uint32_t Id_;
	std::pair<double, double> Center_;
	std::pair<double, double> FirstPoint_;

	double GetRadius() const {
		return NGeomAlgo::DotDistance(Center_, FirstPoint_);
	}
};

class Hexagon : public IFigure {
public:
	Hexagon(): Id_(0), Center_(std::make_pair(0, 0)), FirstPoint_(std::make_pair(0, 0))
	{
	}

	Hexagon(uint32_t id, std::pair<double, double> center, std::pair<double, double> v1):
		Id_(id), Center_(std::move(center)), FirstPoint_(std::move(v1))
	{
	}

	uint32_t Id() const override {
		return Id_;
	}

	double Square() const override {
		return NGeomAlgo::NGonSquare(6, GetRadius());
	}

	std::pair<double, double> Center() const override {
		return Center_;
	}

	std::ostream& Print(std::ostream& out) const override {
		out << "ID: " << Id_ << "\n";
		out << "Figure: Hexagon" << "\n";
		out << "Coors:\n";
		std::vector<std::pair<double, double>> coors = NGeomAlgo::GetNGonCoors(6, Center_, FirstPoint_);
		for (auto& p : coors)
			out << p << "\n";
		return out;
	}

	void SerializeJson(std::ofstream& out) const override {
		out << "{\"id\": " << Id_ << ",";
		out << "\"figure\": \"Hexagon\",";
		out << "\"coors\": [";
		std::vector<std::pair<double, double>> coors = NGeomAlgo::GetNGonCoors(6, Center_, FirstPoint_);
		for (int i = 0; i < 6; ++i) {
			out << "[" << coors[i].first << ", " << coors[i].second << "]" << "\n";
			if (i != 5)
				out << ",";
		}
		out << "]}";
	}

	void Serialize(std::ofstream& os) const override {
		FigureType type = HEXAGON;
		os.write(reinterpret_cast<char*>(&type), sizeof(type));
		os.write((char*)(&Id_), sizeof(Id_));
		os.write((char*)(&Center_.first), sizeof(Center_.first));
		os.write((char*)(&Center_.second), sizeof(Center_.second));
		os.write((char*)(&FirstPoint_.first), sizeof(FirstPoint_.first));
		os.write((char*)(&FirstPoint_.second), sizeof(FirstPoint_.second));
	}

	void Deserialize(std::ifstream& is) override {
		is.read((char*)(&Id_), sizeof(Id_));
		is.read((char*)(&Center_.first), sizeof(Center_.first));
		is.read((char*)(&Center_.second), sizeof(Center_.second));
		is.read((char*)(&FirstPoint_.first), sizeof(FirstPoint_.first));
		is.read((char*)(&FirstPoint_.second), sizeof(FirstPoint_.second));
	}

private:
	uint32_t Id_;
	using TVertex = std::pair<double, double>;
	TVertex Center_;
	TVertex FirstPoint_;

	double GetRadius() const {
		return NGeomAlgo::DotDistance(Center_, FirstPoint_);
	}

};

class IFactory {
public:
	using TVertex = std::pair<double, double>;
	virtual std::shared_ptr<IFigure> FigureCreate(uint32_t id, TVertex center, TVertex v1) const = 0;
	virtual std::shared_ptr<IFigure> FigureCreate() const = 0;
};

class RhombusFactory : public IFactory {
public:
	std::shared_ptr<IFigure> FigureCreate(uint32_t id, TVertex center, TVertex v1) const override {
		return std::shared_ptr<IFigure>(new Rhombus(id, center, v1));
	}

	std::shared_ptr<IFigure> FigureCreate() const override {
		return std::shared_ptr<IFigure>(new Rhombus());
	}
};

class PentagonFactory : public IFactory {
public:
	std::shared_ptr<IFigure> FigureCreate(uint32_t id, TVertex center, TVertex v1) const override {
		return std::shared_ptr<IFigure>(new Pentagon(id, center, v1));
	}

	std::shared_ptr<IFigure> FigureCreate() const override {
		return std::shared_ptr<IFigure>(new Pentagon());
	}
};

class HexagonFactory : public IFactory {
public:
	std::shared_ptr<IFigure> FigureCreate(uint32_t id, TVertex center, TVertex v1) const override {
		return std::shared_ptr<IFigure>(new Hexagon(id, center, v1));
	}

	std::shared_ptr<IFigure> FigureCreate() const override {
		return std::shared_ptr<IFigure>(new Hexagon());
	}
};


#endif
