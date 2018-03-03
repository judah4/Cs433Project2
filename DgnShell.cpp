#include "DgnShell.h"



DgnShell::DgnShell()
{
	exitStatus = false;
	histIndex = 0;
	concatCommand = "";
	argsIndex = 0;
	std::cout << "     ::     UNIX SHELL    :: " << std::endl;
	std::cout << std::endl;
	std::cout << userName + "@" + hostName + ":~/";
}


DgnShell::~DgnShell()
{
}


void DgnShell::SendArgs(std::string check, char * args[]) { ///get a command from user using istream and ' ' as delimiter


	concatCommand = "";  //re-init values
	argsIndex = 0;


	if (check[0] == 'e'&&check[1] == 'x'&&check[2] == 'i'&&check[3] == 't') { ///if user enters exit, set exit bool to true

		exitStatus = true;
		check = "";
		//
		return;

	}
	else if (check[0] == '!'&&check[1] != '!'&&check.length() != 1 && exitStatus == false && check != "exit")     ///checks for !# expressions
	{

		this->histArg(check, args); ///defined below
									// return;
		return;
	}



	else if (check == "!"&&check != "exit") {  ///error handling
		m_history.push_back(check);
		histIndex++;
		return;
	}
	else if (check == "cd.." || check == "cd.") {
		m_history.push_back(check);
		histIndex++;
		return;
	}


	else if (check == "cd"&&exitStatus == false && check != "exit") { /// home directory chdir handling
		m_history.push_back(check);
		histIndex++;
		//  std::cout<<"HOMEDIR CHECK"<<std::endl;
		std::string myText(homedir);
		std::istringstream iss(myText);
		std::string token;
		argsIndex = 0;
		while (getline(iss, token, ' '))
		{
			args[argsIndex] = strdup(token.c_str());
			argsIndex++;
		}
		args[argsIndex] = '\0';

		int directory = chdir(args[0]);  ///check for valid directory

		if (directory == -1) {
			perror("");

		}
		return;
	}
	///end homdir cd check



	if (check == "m_history"&&exitStatus == false) ///if the user enters "m_history", display m_history with number vals
	{



		int histcount = m_history.size();
		if (m_history.size() == 0) { std::cout << "[empty]" << std::endl; }

		else if (m_history.size() == 1) {
			std::cout << "1 " << m_history[0] << std::endl;

		}
		else if (m_history.size()>10)
		{
			int histcount = m_history.size();                  ///if m_history greater than 10, only show the previous ten entries
			int j = histcount - 1;
			for (int i = 0; i<10; i++)
			{
				std::cout << histcount-- << " " << m_history[j] << std::endl;
				j--;
			}
		}
		else
		{       //display m_history
			for (std::vector<std::string>::reverse_iterator
				i = m_history.rbegin();
				i != m_history.rend(); ++i)
			{
				std::cout << histcount-- << " " << *i << std::endl;
			}
		}


		return;
	}






	else if (check == "!!"&&exitStatus == false) { //check for !! case, execute previous command


		if (m_history.size() == 0) {
			std::cout << "No commands in m_history" << std::endl;
			return;
		}
		check = m_history[histIndex - 1];
		//std::cout<<"CHECK IS "<<check<<std::endl;
		if (m_history[histIndex - 1] == "cd") {   //if prev m_history is cd, chdir to homdir

												//  std::cout<<"HOMEDIR CHECK"<<std::endl;
			std::string myText(homedir);
			std::istringstream iss(myText);
			std::string token;
			argsIndex = 0;
			while (getline(iss, token, ' '))
			{
				args[argsIndex] = strdup(token.c_str());
				argsIndex++;
			}
			args[argsIndex] = '\0';

			int directory = chdir(args[0]);  ///check for valid directory

			if (directory == -1) {
				perror("");

			}

			argsIndex = 0;

			return;
		}
		if (exitStatus == false && check != "cd.."&&check != "exit") {   //case fore regular previous m_history input


			argsIndex = 0;


			std::string myText(m_history[histIndex - 1]);

			std::istringstream iss(myText);
			std::string token;
			while (getline(iss, token, ' '))
			{
				args[argsIndex] = strdup(token.c_str());
				argsIndex++;
			}
			args[argsIndex] = NULL;


			if (check[0] == 'c' && check[1] == 'd'&&check != "cd"&&exitStatus == false)
			{

				args[argsIndex] = '\0';

				int directory = chdir(args[1]);  ///check for valid directory

				if (directory == -1) {
					perror("");

				}

				return;

			}

			if (check[0] != 'l'&&check[1] != 's')
			{
				check = args[argsIndex - 1];
			}
			if (check == "&")
			{
				args[argsIndex - 1] = '\0';          //dont wait for child check


			}



			m_pid = fork();
			if (m_pid < 0) { /* error occurred */
				fprintf(stderr, "Fork Failed");
				//return 1;
			}
			else if (pid == 0) { /* child process */

				execvp(args[0], args);
				exit(EXIT_FAILURE);  //if the child process fails, exit the child process.
			}
			else if (check != "&") {/* parent will wait for the child to complete */

				wait(NULL);

			}
			else
			{
				return;
			}

		}
		return;
	}//end !! check


	if (check == "exit") { ///if user enters exit, set exit bool to true

		exitStatus = true;
		std::cout << "exit = true" << std::endl;
		std::cout << "attempting exit" << std::endl;
		//  this->getArgs(args);
		return;
	}

	if (check != "exit") {
		this->regularCheck(check, args);         ///check a normal, non chdir or m_history function input
		return;
	}
	// std::cout<<"END"<<std::endl;
	return;

};

void DgnShell::initShell() {       //initialize the shell and display a prompt

	histIndex = 0;
	argsIndex = 0;

};


void DgnShell::histArg(std::string check, char * args[]) {//handle user m_history selection
													   /// memset(&args[0], '\0', sizeof(args)); //neccesary?
													   // std::cout<<"CHECK IS::" << check << std::endl;




	if (check[0] != 'c'&& check[1] != 'd'&& exitStatus == false) {   ///handle non chdir cases

																	 // std::cout<<"History size ::"<<m_history.size()<<std::endl;
																	 // std::cout<<"History index ::"<<histIndex<<std::endl;


		int numberwang = atoi(check.substr(1, 2).c_str());

		if (m_history.size() == 0)
		{
			std::cout << "No such command in m_history" << std::endl;
			return;
		}
		else if (numberwang>(histIndex)) {
			std::cout << "No such command in m_history" << std::endl;
			return;
		}




		check = m_history[numberwang - 1];

		if (check == "!") {                       ///error handling
			m_history.push_back(check);
			histIndex++;
			return;
		}
		else if (check == "cd.." || check == "cd.") {
			m_history.push_back(check);
			histIndex++;
			return;
		}

		if (check == "cd") {////if previous m_history entry == cd

							//  std::cout<<"HOMEDIR CHECK"<<std::endl;
			std::string myText(homedir);
			std::istringstream iss(myText);
			std::string token;
			argsIndex = 0;
			while (getline(iss, token, ' '))
			{
				args[argsIndex] = strdup(token.c_str());
				argsIndex++;
			}
			args[argsIndex] = '\0';

			int directory = chdir(args[0]);  ///check for valid directory

			if (directory == -1) {
				perror(" ");

			}
			return;
		}


		std::string myText(check);
		std::istringstream iss(myText);
		std::string token;
		argsIndex = 0;
		while (getline(iss, token, ' '))
		{
			args[argsIndex] = strdup(token.c_str());
			argsIndex++;

		}
		args[argsIndex] = '\0';




		/////////////////////////////////////////////////////
		if (check[0] == 'c' && check[1] == 'd'&& check != "cd"&&exitStatus == false)
		{

			// std::cout<< "CD CHECK FOR !# INPUT" << std::endl;

			int directory = chdir(args[1]);  ///check for valid directory

			if (directory == -1) {
				perror("");

			}

			return;
		}





		if (check[0] != '!') {  //if the userinput is not a m_history selection

			if (check[0] != 'l'&&check[1] != 's')
			{
				check = args[argsIndex - 1];
			}

			if (check == "&")
			{
				args[argsIndex - 1] = '\0';          //dont wait for child check


			}



			m_pid = fork();
			if (m_pid < 0) { /* error occurred */
				fprintf(stderr, "Fork Failed");
				//return 1;
			}
			else if (pid == 0) { /* child process */

				execvp(args[0], args);
				exit(EXIT_FAILURE);
			}
			else if (check != "&") {/* parent will wait for the child to complete */

				wait(NULL);

			}
			else
			{

			}
			//return;

		};
		///////////////////////////////////////////////////////////////////////////////////

	}//end if not cd
	 //std::cout << "REACHED END " << std::endl;

	return;////////////////////////////////////////////////////

};


void DgnShell::regularCheck(std::string check, char * args[]) {  ///checks for normal user input and pushes to args for exec
															  //std::cout<<check<<std::endl;

	if (check != "exit") {


		if (check.size() == 0 && exitStatus == false) {  ///check for empty input


			return;
		}
		else if (exitStatus == false && check != "cd.."&&check != "exit") {

			argsIndex = 0;
			std::string myText(check);
			std::istringstream iss(myText);
			std::string token;
			argsIndex = 0;
			while (getline(iss, token, ' '))
			{
				concatCommand.append(token + " ");
				args[argsIndex] = strdup(token.c_str());
				argsIndex++;

			}
			args[argsIndex] = '\0';

			if (check != "m_history"&&exitStatus == false && check != "!!"&&check[0] != '!')
			{

				m_history.push_back(concatCommand);///push user string to m_history
				histIndex++;
				concatCommand = "";


			}



		}





		///CD CHECK
		if (check[0] == 'c' && check[1] == 'd'&& exitStatus == false && check != "cd")
		{

			int directory = chdir(args[1]);  ///check for valid directory

			if (directory == -1) {
				perror("");

			}

			return;
		}

		if (exitStatus == false && check != "cd..")
		{

			///fork a child process
			if (check[0] != 'l'&&check[1] != 's')
			{
				check = args[argsIndex - 1];
			}
			//  std::cout<< check<<std::endl;
			if (check == "&")
			{
				args[argsIndex - 1] = '\0';


			}

			/// std::cout << check + "::final then fork"<< std::endl;

			m_pid = fork();


			if (m_pid < 0) { /* error occurred */
				fprintf(stderr, "Fork Failed");
				//return 1;
			}
			else if (pid == 0) { /* child process */

				execvp(args[0], args);
				perror("");
				exit(EXIT_FAILURE);
			}
			else if (check != "&") {/* parent will wait for the child to complete */

				wait(NULL);


				//  std::cout <<"Child Complete" << std::endl;
			}
			else
			{

			}
			return;
		}


	}//endif check=exit

}

bool DgnShell::getExitStatus() {
	return exitStatus;
}



