**Custom Shell Program**
Code Status
The shell program is functional with essential built-in commands like cd, exit, jobs, kill, and help. The shell also supports the execution of external commands and the management of background processes. However, some features may have limitations or known bugs (see below).

Known Bugs
Occasionally, background processes may not terminate correctly, which affects the job list display.
Inconsistent handling of the kill command for specific background processes when the job list is full.
The command parser might misinterpret commands with special characters, requiring careful input syntax.
Implemented Features
Built-in Commands:

cd <directory> - Change current directory
exit - Exit the shell
jobs - List all background jobs
kill <job_number> - Terminate a background job by job number
help - Display information about available commands
Background Process Handling: Execute commands in the background by appending &, with active background processes added to a job list.

Error Handling: Each command includes error messages for invalid inputs or failed operations.

Additional Features
If you added any additional features, list them here. For example:

Enhanced Prompt Display: Custom shell prompt displays the current working directory.
Command History (if implemented): Allows users to view and execute previously entered commands.
Acknowledgments
Special thanks to:

[Helper/Resource Name] - for guidance on process management in Unix.
Online resources like Stack Overflow and GeeksforGeeks for syntax examples and debugging tips.
Friends/classmates who provided valuable insights during the development process.
