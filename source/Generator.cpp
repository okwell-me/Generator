#include<iostream>
#include<string>
#include<Windows.h>



//#define AUTO
#define MANUAL
using namespace std;

enum class SysState {
	SysOff,
	FindCloud,
	SysErr,
};

enum class GenState {
	GenOff,
	GenOn
};

class CommandStrategy {
public:
	virtual string getCommand() const = 0;
};

class ConsoleCommandStrategy : public CommandStrategy {
public:
	virtual string getCommand() const override {
		string cmd;
		cin >> cmd;
		return cmd;
	}
};

class System {
public:
	SysState state;
	GenState genstate;
	int parseCmd(const string& command);
	void processEvent(GenState& genstate);
	void processEvent(int command, SysState& systate, GenState& genstate);
	bool FindClouds(/**/);
	void EnableGenerator(GenState& genstate);
	void DisableGenerator(GenState& genstate);
	CommandStrategy* strat;
	System(CommandStrategy* strat) {}
	~System() {
		delete strat;
	}
	string getCommand() {
		string cmd;
		cin >> cmd;
		return cmd;
	}
};

void System::EnableGenerator(GenState& genstate) {
	if (genstate == GenState::GenOff) {
		/*gpio_GeneratorPin high*/
		genstate = GenState::GenOn;
		cout << "Generator enabled\n";
	}
	else if (genstate == GenState::GenOn) {
		cout << "generator is already enabled\n";
	}
}

void System::DisableGenerator(GenState& genstate) {
	if (genstate == GenState::GenOn) {
		/*gpio_GeneratorPin low*/
		genstate = GenState::GenOff;
		cout << "Generator disabled\n";
	}
	else if (genstate == GenState::GenOff) {
		cout << "generator is already disabled\n";
	}
}

bool System::FindClouds(/*cv::Mat img*/) {     //opencv finding clouds
	if (/*clouds*/rand() % 2) {
		cout << "Clouds alert!!!\n";
		return true;
	}
	else { /*if no clouds*/
		cout << "Sky is clear\n";
		return false;
	}
}
#ifdef AUTO
void System::processEvent(GenState& genstate) {
	if (FindClouds()) {
		EnableGenerator(genstate);
	}
	else {
		DisableGenerator(genstate);
	}
}
#endif

#ifdef MANUAL
void System::processEvent(int command, SysState& systate, GenState& genstate) {
	switch (command) {
	case 0: // /check_sun
		if (FindClouds()) {
			EnableGenerator(genstate);
		}
		else {
			DisableGenerator(genstate);
		}
		break;
	case 1: // gen_on
		EnableGenerator(genstate);
		break;
	case 2: // /gen_off
		DisableGenerator(genstate);
		break;
	case 3: // /sys_off
		DisableGenerator(genstate);
		systate = SysState::SysOff;
		cout << "System is off\n";
		break;
	}
}

int System::parseCmd(const string& command) {
	if (command == "/check_sun") {
		return 0;
	}
	else
		if (command == "/gen_on") {
			return 1;
		}
		else
			if (command == "/gen_off") {
				return 2;
			}
			else
				if (command == "/sys_off") {
					return 3;
				}
				else {
					cout << "Wrong command!\n";
				}
}
#endif

int main() {
	srand(time(NULL));
	System system(new ConsoleCommandStrategy());
	system.state = SysState::FindCloud;
	system.genstate = GenState::GenOff;

	while (system.state != SysState::SysOff) {
#ifdef MANUAL
		string cmd = system.getCommand();
		system.processEvent(system.parseCmd(cmd), system.state, system.genstate);
#endif
#ifdef AUTO
		system.processEvent(system.genstate);
		Sleep(1000);
#endif
	}
	return 0;
}