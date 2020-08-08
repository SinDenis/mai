#include <iostream>
#include "src/Editor.h"

void help() {
	std::cout << "help\n";
	std::cout << "Create\n";
	std::cout << "Load\n";
  	std::cout << "Save <filename>\n";
  	std::cout << "Add <FigureType Center Point>\n";
  	std::cout << "Remove <Figure ID>\n";
  	std::cout << "Undo\n";
  	std::cout << "Print\n";
}

void create(Editor& editor) {
	std::string cmd;
	if (editor.DocumentExist()) {
		std::cout << "Save old document or don't save? Yes/No\n";
		std::cin >> cmd;
		if (cmd == "Yes") {
			std::string filename;
			std::cout << "Enter name of file\n";
			std::cin >> filename;
			try {
				editor.SaveDocument(filename);
				std::cout << "Document save in file " << filename << "\n";
			} catch (std::runtime_error& err) {
				std::cout << err.what() << "\n";
			}
		}
		std::cout << "Enter name of new project\n";
	}
	std::cin >> cmd;
	editor.CreateDocument(cmd);
	std::cout << "Document " << cmd << " is created\n";
}

void load(Editor& editor) {
	std::string cmd;
	if (editor.DocumentExist()) {
		std::cout << "Save old document or don't save? Yes/No\n";
		std::cin >> cmd;
		if (cmd == "Yes") {
			std::string filename;
			std::cout << "Enter name of file\n";
			std::cin >> filename;
			try {
				editor.SaveDocument(filename);
				std::cout << "Document save in file " << filename << "\n";
			} catch (std::runtime_error& err) {
				std::cout << err.what() << "\n";
			}
		}
	}
	std::string filename;
	std::cin >> filename;
	try {
		editor.LoadDocument(filename);
		std::cout << "Document loaded from file " << filename << "\n";
	} catch (std::runtime_error& err) {
		std::cout << err.what() << "\n";
	}
}

void save(Editor& editor) {
	std::string filename;
	std::cin >> filename;

	try {
		editor.SaveDocument(filename);
		std::cout << "Document save in file " << filename << "\n";
	} catch (std::runtime_error& err) {
		std::cout << err.what() << "\n";
	}
}

void saveJson(Editor& editor) {
	std::string filename = "../ui/test.json";
	try {
		editor.SaveDocumentJson(filename);
		std::cout << "Document save in file " << filename << "\n";
	} catch (std::runtime_error& err) {
		std::cout << err.what() << "\n";
	}
}

void add (Editor& editor) {
	char type;
	std::pair<double, double> center;
	std::pair<double, double> point;
	std::cin >> type >> center.first >> center.second >> point.first >> point.second;
	switch(type) {
		case 'R':
			editor.InsertPrimitive(RHOMBUS, center, point);
			break;
		case 'P':
			editor.InsertPrimitive(PENTAGON, center, point);
			break;
		case 'H':
			editor.InsertPrimitive(HEXAGON, center, point);
			break;
		default:
			std::cout << "Primitive isn't added\n";
			return ;
	}
	std::cout << "Primitive is added\n";
}

void remove(Editor& editor) {
	uint32_t id;
	std::cin >> id;

	try {
		editor.RemovePrimitive(id);
		std::cout << "Primitive with " << id << " is removed\n";
	} catch (std::logic_error& err) {
		std::cout << err.what() << "\n";
	}

}

int main() {
	Editor editor;
	std::string cmd;

	while (std::cin >> cmd) {
		if (cmd == "help") {
			help();
		} else if (cmd == "Create") {
			create(editor);
		} else if (cmd == "Load") {
			load(editor);
		} else if (cmd == "Save") {
			save(editor);
		} else if (cmd == "Add") {
			add(editor);
		} else if (cmd == "Remove") {
			remove(editor);
		} else if (cmd == "Undo") {
			editor.Undo();
			std::cout << "OK\n";
		} else if (cmd == "Print") {
			editor.PrintDocument();
		} else if (cmd == "Json") {
			saveJson(editor);
		}
	}

	return 0;
}
