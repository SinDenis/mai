//
// Created by Sin Denis on 2019-12-25.
//

#include "src/storage_api.h"

void put(NStoreAPI::StorageAPI& api) {
	int32_t key;
	double val;
	std::cin >> key >> val;
	std::cout << statusToStr(api.put(key, val)) << std::endl;
}

void get(NStoreAPI::StorageAPI& api) {
	int32_t key;
	double val;
	std::cin >> key;
	ResStatus status = api.get(key, val);
	if (status == ResStatus::ok)
		std::cout << "ok: " << val << std::endl;
	else
		std::cout << statusToStr(status) << std::endl;
}

void remove(NStoreAPI::StorageAPI& api) {
	int32_t key;
	std::cin >> key;
	std::cout << statusToStr(api.remove(key)) << std::endl;
}

int main()
{
	NStoreAPI::StorageAPI storageApi(30000, 30001);
	std::string cmd;
	while (std::cin >> cmd) {
		if (cmd == "put") {
			put(storageApi);
		} else if (cmd == "remove") {
			remove(storageApi);
		} else if (cmd == "get") {
			get(storageApi);
		} else if (cmd == "quit") {
			break;
		} else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			std::cout << "Wrong input" << std::endl;
		}
	}
	return 0;
}
