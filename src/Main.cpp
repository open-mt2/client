#include <Client/Application/Application.h>

//       ____                   __  __ _______ ___     _____ _ _            _
//      / __ \                 |  \/  |__   __|__ \   / ____| (_)          | |
//     | |  | |_ __   ___ _ __ | \  / |  | |     ) | | |    | |_  ___ _ __ | |_
//     | |  | | '_ \ / _ \ '_ \| |\/| |  | |    / /  | |    | | |/ _ \ '_ \| __|
//     | |__| | |_) |  __/ | | | |  | |  | |   / /_  | |____| | |  __/ | | | |_
//      \____/| .__/ \___|_| |_|_|  |_|  |_|  |____|  \_____|_|_|\___|_| |_|\__|
//            | |
//            |_|
//

int main(int argc, char** argv)
{
	OMT2Client::Application::Application application;
	return application.Run(argc, argv);
}
