//
// Created by Sin Denis on 2019-11-26.
//

#ifndef EDITOR_H
#define EDITOR_H

#include "Document.h"
#include "Command.h"
#include <stack>

class Editor {
public:
	Editor() : Doc_(nullptr), History_()
	{
	}

	void CreateDocument(const std::string& name) {
		Doc_ = std::make_shared<Document>(name);
	}

	void InsertPrimitive(FigureType type, const std::pair<double, double>& center, const std::pair<double, double>& point) {
		std::shared_ptr<ICommand> command = std::shared_ptr<ICommand>(new InsertCommand(type, center, point));
		command->SetDocument(Doc_);
		command->Execute();
		History_.push(command);
	}

	void RemovePrimitive(uint32_t id) {
		std::shared_ptr<ICommand> command = std::shared_ptr<ICommand>(new RemoveCommand(id));
		command->SetDocument(Doc_);
		command->Execute();
		History_.push(command);
	}

	void SaveDocument(const std::string& filename) {
		Doc_->Save(filename);
	}

	void SaveDocumentJson(const std::string& filename) {
		Doc_->SaveJson(filename);
	}

	void LoadDocument(const std::string& filename) {
		Doc_ = std::make_shared<Document>("NoName");
		Doc_->Load(filename);
	}

	void Undo() {
		if (History_.empty()) {
			throw std::logic_error("History is empty");
		}
		std::shared_ptr<ICommand> lastCommand = History_.top();
		lastCommand->UnExecute();
		History_.pop();
	}

	void PrintDocument() {
		Doc_->Print();
	}

	bool DocumentExist() {
		return Doc_ != nullptr;
	}

	~Editor() = default;
private:
	std::shared_ptr<Document> Doc_;
	std::stack<std::shared_ptr<ICommand>> History_;
};

#endif
