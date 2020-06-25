// CONSOLE.CPP
//

#include "imgui.h"
#include "FUNCTION.H"

char console_text[16384];
int console_text_len = 0;
char command_text[512];

#define	MAX_ARGS		80

static	int			cmd_argc;
static	char* cmd_argv[MAX_ARGS];
static	char* cmd_null_string = "";
static	char* cmd_args = NULL;

char	com_token[1024];
int		com_argc;
char** com_argv;

typedef struct cmd_function_s
{
	struct cmd_function_s* next;
	char* name;
	xcommand_t				function;
} cmd_function_t;

static bool executeConsoleCommand = false;

static	cmd_function_t* cmd_functions;		// possible commands to execute

/*
============
Cmd_AddCommand
============
*/
void	Cmd_AddCommand(char* cmd_name, xcommand_t function)
{
	cmd_function_t* cmd;

	// fail if the command already exists
	for (cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if (!strcmp(cmd_name, cmd->name))
		{
			Console_Printf("Cmd_AddCommand: %s already defined\n", cmd_name);
			return;
		}
	}

	cmd = (cmd_function_t *)malloc(sizeof(cmd_function_t));
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next = cmd_functions;
	cmd_functions = cmd;
}


/*
==============
Cmd_Argc
==============
*/
int Cmd_Argc() {
	return cmd_argc;
}

/*
==============
Cmd_Argv
==============
*/
char* Cmd_Argv(int index) {
	return cmd_argv[index];
}

/*
==============
COM_Parse

Parse a token out of a string
==============
*/
char* COM_Parse(char* data)
{
	int             c;
	int             len;

	len = 0;
	com_token[0] = 0;

	if (!data)
		return NULL;

	// skip whitespace
skipwhite:
	while ((c = *data) <= ' ')
	{
		if (c == 0)
			return NULL;                    // end of file;
		data++;
	}

	// skip // comments
	if (c == '/' && data[1] == '/')
	{
		while (*data && *data != '\n')
			data++;
		goto skipwhite;
	}


	// handle quoted strings specially
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c == '\"' || !c)
			{
				com_token[len] = 0;
				return data;
			}
			com_token[len] = c;
			len++;
		}
	}

	// parse single characters
	if (c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ':')
	{
		com_token[len] = c;
		len++;
		com_token[len] = 0;
		return data + 1;
	}

	// parse a regular word
	do
	{
		com_token[len] = c;
		data++;
		len++;
		c = *data;
		if (c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ':')
			break;
	} while (c > 32);

	com_token[len] = 0;
	return data;
}


/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
============
*/
void Cmd_TokenizeString(char* text)
{
	int		i;

	// clear the args from the last string
	for (i = 0; i < cmd_argc; i++)
		free(cmd_argv[i]);

	cmd_argc = 0;
	cmd_args = NULL;

	while (1)
	{
		// skip whitespace up to a /n
		while (*text && *text <= ' ' && *text != '\n')
		{
			text++;
		}

		if (*text == '\n')
		{	// a newline seperates commands in the buffer
			text++;
			break;
		}

		if (!*text)
			return;

		if (cmd_argc == 1)
			cmd_args = text;

		text = COM_Parse(text);
		if (!text)
			return;

		if (cmd_argc < MAX_ARGS)
		{
			cmd_argv[cmd_argc] = (char *)malloc(strlen(com_token) + 1);
			strcpy(cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		}
	}

}

/*
====================
Console_Init
====================
*/
void Console_Init(void) {
	memset(command_text, 0, sizeof(command_text));
	memset(console_text, 0, sizeof(console_text));
	console_text_len = 0;
}

/*
====================
Console_Render
====================
*/
void Console_Printf(const char* fmt, ...) {
	va_list		argptr;
	char		msg[4096];

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	//int len = strlen(msg);
	//strcpy(&console_text[console_text_len], msg);

	OutputDebugStringA(msg);
	//console_text_len += len;
}

/*
====================
ExecuteConsoleCommand
====================
*/
void ExecuteConsoleCommand(void) {
	cmd_function_t* cmd;

	if (strlen(command_text) <= 0)
		return;

	Console_Printf("%s\n", command_text);
	Cmd_TokenizeString(command_text);

	memset(command_text, 0, sizeof(command_text));

	for (cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if (!strcmp(Cmd_Argv(0), cmd->name))
		{
			cmd->function();			
			return;
		}
	}

	Console_Printf("Unknown command %s\n", Cmd_Argv(0));
}

/*
====================
Console_Render
====================
*/
void Console_Render(void) {
	bool ScreenActive;	

	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2(0, 0);
	ImGui::SetNextWindowSize(ImVec2(420, 270));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Developer Console", &ScreenActive, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		ImVec2 textres(420, 180);
		ImGui::PushID("ConsoleTxt");
			ImGui::InputTextMultiline("", &console_text[0], sizeof(console_text), textres, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopID();
		ImGui::Text("Commands");
		ImGui::PushID("CmdTxt");
			ImGui::InputText("", &command_text[0], sizeof(command_text));
		ImGui::PopID();
		ImGui::SameLine();
		if (ImGui::Button("Execute")) {
			executeConsoleCommand = true;			
		}
	ImGui::End();
}

/*
=================
Console_Tick
=================
*/
void Console_Tick(void) {
	if (executeConsoleCommand) {
		ExecuteConsoleCommand();
		executeConsoleCommand = false;
	}
}