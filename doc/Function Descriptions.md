
--------------------
### readline

Prototype:
```C
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

char *readline (const char *prompt);
```
Description:

	readline will read a line from the terminal and return it, using
	prompt as a prompt. If prompt is NULL or the empty string, no
	prompt is issued. The line returned is allocated with malloc(3);
	the caller must free it when finished. The line returned has the
	final newline removed, so only the text of the line remains.

Return:

	readline returns the text of the line read. A blank line returns
	the empty string. If EOF is encountered while reading a line,
	and the line is empty, NULL is returned. If an EOF is read with
	a non-empty line, it is treated as a newline.

--------------------

### rl_clear_history

Prototype:
```C
#include <readline/readline.h>

void rl_clear_history(void);
```
Description:

	Clear the history list by deleting all of the entries, in the
	same manner as the History library’s clear_history() function.
	This differs from clear_history because it frees private data
	Readline saves in the history list.

--------------------

### rl_on_new_line

Prototype:
```C
#include <readline/readline.h>

int rl_on_new_line(void);
```
Description:

	Tell the update functions that we have moved onto a new (empty)
	line, usually after outputting a newline.

--------------------

### rl_replace_line

Prototype:
```C
#include <readline/readline.h>

void rl_replace_line(const char *text, int clear_undo);
```
Description:

	Replace the contents of rl_line_buffer with text. The point and
	mark are preserved, if possible. If clear_undo is non-zero, the
	undo list associated with the current line is cleared.

--------------------

### rl_redisplay

Prototype:
```C
#include <readline/readline.h>

void rl_redisplay(void);
```
Description:

	Change what’s displayed on the screen to reflect the current
	contents of rl_line_buffer.

--------------------

### add_history

Prototype:
```C
#include <readline/history.h>

void add_history(const char *string);
```
Description:

	Place string at the end of the history list. The associated data
	field (if any) is set to NULL. If the maximum number of history
	entries has been set using stifle_history(), and the new number
	of history entries would exceed that maximum, the oldest history
	entry is removed.

--------------------

### access

Prototype:
```C
#include <unistd.h>

int access(const char *pathname, int mode);
```
Description:

	access() checks whether the calling process can access the file
	pathname. If pathname is a symbolic link, it is dereferenced.

	The mode specifies the accessibility check(s) to be performed,
	and is either the value F_OK, or a mask consisting of the bitwise
	OR of one or more of R_OK, W_OK, and X_OK. F_OK tests for the
	existence of the file. R_OK, W_OK, and X_OK test whether the
	file exists and grants read, write, and execute permissions,
	respectively.


Return:

	On success zero is returned. On error -1 is returned,
	and errno is set to indicate the error.

--------------------

### wait, waitpid

Prototype:
```C
#include <sys/wait.h>

pid_t wait(int *_Nullable wstatus);
pid_t waitpid(pid_t pid, int *_Nullable wstatus, int options);
```
Description:

	wait for state changes in a child of the calling process, and
	obtain information about the child whose state has changed.
	A state change is considered to be: the child terminated; the
	child was stopped by a signal; or the child was resumed by a
	signal. In the case of a terminated child, performing a wait
	allows the system to release the resources associated with the
	child

	If a child has already changed state, then these calls return
	immediately. Otherwise, they block until either a child changes
	state or a signal handler interrupts the call

	The wait() system call suspends execution of the calling thread
	until one of its children terminates. The call wait(&wstatus) is
	equivalent to:
		waitpid(-1, &wstatus, 0);

	The waitpid() system call suspends execution of the calling
	thread until a child specified by pid argument has changed state.
	By default, waitpid() waits only for terminated children, but
	this behavior is modifiable via the options argument, as
	described below.

Return:

	wait(): on success, returns the process ID of the terminated
	child; on failure, -1 is returned.

	waitpid(): on success, returns the process ID of the child whose
	state has changed; if WNOHANG was specified and one or more
	child(ren) specified by pid exist, but have not yet changed
	state, then 0 is returned. On failure, -1 is returned.

--------------------

### wait3, wait4

Prototype:
```C
#include <sys/wait.h>

pid_t wait3(int *_Nullable wstatus, int options,
			struct rusage *_Nullable rusage);
pid_t wait4(pid_t pid, int *_Nullable wstatus, int options,
			struct rusage *_Nullable rusage);
```
Description:

	Similar to wait and waitpid accordingly, but in addition return
	resource usage information about the child in the structure
	pointed to by rusage.

--------------------

### getcwd

Prototype:
```C
#include <unistd.h>

char *getcwd(char buf[.size], size_t size);
```
Description:

	These functions return a null-terminated string containing an
	absolute pathname that is the current working directory of the
	calling process. The pathname is returned as the function result
	and via the argument buf, if present.

	The getcwd() function copies an absolute pathname of the current
	working directory to the array pointed to by buf, which is of
	length size.

	If the length of the absolute pathname of the current working
	directory, including the terminating null byte, exceeds size
	bytes, NULL is returned, and errno is set to ERANGE; an
	application should check for this error, and allocate a larger
	buffer if necessary.

	As an extension to the POSIX.1-2001 standard, glibc's getcwd()
	allocates the buffer dynamically using malloc(3) if buf is NULL.
	In this case, the allocated buffer has the length size unless
	size is zero, when buf is allocated as big as necessary. The
	caller should free(3) the returned buffer.

--------------------

### chdir

Prototype:
```C
#include <unistd.h>

int chdir(const char *path);
```
Description:

	chdir() changes the current working directory of the calling
	process to the directory specified in path.

Return:

	On success, zero is returned. On error, -1 is returned, and
	errno is set to indicate the error.

--------------------

### stat, lstat, fstat

Prototype:
```C
#include <sys/stat.h>

int stat(const char *restrict pathname
			struct stat *restrict statbuf);
int fstat(int fd, struct stat *statbuf);
int lstat(const char *restrict pathname,
			struct stat *restrict statbuf);
```
Description:

	These functions return information about a file, in the buffer
	pointed to by statbuf. No permissions are required on the file
	itself, but permission is required on all of the directories in
	pathname that lead to the file.

	stat() retrieves information about the file pointed to by
	pathname;

	lstat() is identical to stat(), except that if pathname is a
	symbolic link, then it returns information about the link itself,
	not the file that the link refers to.

	fstat() is identical to stat(), except that the file about which
	information is to be retrieved is specified by the file
	descriptor fd.
```C
struct stat {
		dev_t      st_dev;      /* ID of device containing file */
		ino_t      st_ino;      /* Inode number */
		mode_t     st_mode;     /* File type and mode */
		nlink_t    st_nlink;    /* Number of hard links */
		uid_t      st_uid;      /* User ID of owner */
		gid_t      st_gid;      /* Group ID of owner */
		dev_t      st_rdev;     /* Device ID (if special file) */
		off_t      st_size;     /* Total size, in bytes */
		blksize_t  st_blksize;  /* Block size for filesystem I/O */
		blkcnt_t   st_blocks;   /* Number of 512 B blocks allocated */

		/* Since POSIX.1-2008, this structure supports nanosecond
		precision for the following timestamp fields.
		For the details before POSIX.1-2008, see VERSIONS. */

		struct timespec  st_atim;  /* Time of last access */
		struct timespec  st_mtim;  /* Time of last modification */
		struct timespec  st_ctim;  /* Time of last status change */

	#define st_atime  st_atim.tv_sec  /* Backward compatibility */
	#define st_mtime  st_mtim.tv_sec
	#define st_ctime  st_ctim.tv_sec
};
```
Return:

	On success, zero is returned. On error, -1 is returned, and
	errno is set to indicate the error.

--------------------

### unlink

Prototype:
```C
#include <unistd.h>

int unlink(const char *pathname);
```
Description:

	deletes a name from the filesystem

Return:

	On success, zero is returned. On error, -1 is returned, and
	errno is set to indicate the error.

--------------------

### opendir

Prototype:
```C
#include <sys/types.h>
#include <dirent.h>

DIR *opendir(const char *name);
```
Description:

	The opendir() function opens a directory stream corresponding to
	the directory name, and returns a pointer to the directory
	stream. The stream is positioned at the first entry in the
	directory.

Return:

	On success, return a pointer to the directory stream. On error,
	NULL is returned, and errno is set to indicate the error.

--------------------

### readdir

Prototype:
```C
#include <sys/types.h>
#include <dirent.h>

struct dirent *readdir(DIR *dirp);
```
Description:

	The readdir() function returns a pointer to a dirent structure
	representing the next directory entry in the directory stream
	pointed to by dirp. It returns NULL on reaching the end of the
	directory stream or if an error occurred.

	In the glibc implementation, the dirent structure is defined as
	follows:
```C
struct dirent {
	ino_t          d_ino;       /* Inode number */
	off_t          d_off;       /* Not an offset; see below */
	unsigned short d_reclen;    /* Length of this record */
	unsigned char  d_type;      /* Type of file; not supported
								by all filesystem types */
	char           d_name[256]; /* Null-terminated filename */
};
```
	The only fields in the dirent structure that are mandated by
	POSIX.1 are d_name and d_ino. The other fields are
	unstandardized, and not present on all systems; see NOTES below
	for some further details.

	The fields of the dirent structure are as follows:

	d_ino  This is the inode number of the file.

	d_off  The value returned in d_off is the same as would be
		returned by calling telldir(3) at the current position in
		the directory stream. Be aware that despite its type and
		name, the d_off field is seldom any kind of directory
		offset on modern filesystems. Applications should treat
		this field as an opaque value, making no assumptions about
		its contents; see also telldir(3).

	d_reclen
		This is the size (in bytes) of the returned record. This
		may not match the size of the structure definition shown
		above; see NOTES.

	d_type This field contains a value indicating the file type,
		making it possible to avoid the expense of calling
		lstat(2) if further actions depend on the type of the
		file.

		When a suitable feature test macro is defined
		(_DEFAULT_SOURCE since glibc 2.19, or _BSD_SOURCE on glibc
		2.19 and earlier), glibc defines the following macro
		constants for the value returned in d_type:

		DT_BLK This is a block device.

		DT_CHR This is a character device.

		DT_DIR This is a directory.

		DT_FIFO
				This is a named pipe (FIFO).

		DT_LNK This is a symbolic link.

		DT_REG This is a regular file.

		DT_SOCK
				This is a UNIX domain socket.

		DT_UNKNOWN
				The file type could not be determined.

		Currently, only some filesystems (among them: Btrfs, ext2,
		ext3, and ext4) have full support for returning the file
		type in d_type. All applications must properly handle a
		return of DT_UNKNOWN.

	d_name This field contains the null terminated filename. See
		NOTES.

	The data returned by readdir() may be overwritten by subsequent
	calls to readdir() for the same directory stream.

Return:

	On success, readdir() returns a pointer to a dirent structure.
	(This structure may be statically allocated; do not attempt to
	free(3) it.)

	If the end of the directory stream is reached, NULL is returned
	and errno is not changed. If an error occurs, NULL is returned
	and errno is set to indicate the error. To distinguish end of
	stream from an error, set errno to zero before calling readdir()
	and then check the value of errno if NULL is returned.

--------------------

### closedir

Prototype:
```C
#include <sys/types.h>
#include <dirent.h>

int closedir(DIR *dirp);
```
Description:

	The closedir() function closes the directory stream associated
	with dirp. A successful call to closedir() also closes the
	underlying file descriptor associated with dirp. The directory
	stream descriptor dirp is not available after this call.

Return:

	The closedir() function returns 0 on success. On error, -1 is
	returned, and errno is set to indicate the error.

--------------------

### isatty

Prototype:
```
#include <unistd.h>

int isatty(int fd);
```
Description:

	The isatty() function tests whether fd is an open file descriptor
	referring to a terminal.

Return:

	isatty() returns 1 if fd is an open file descriptor referring to
	a terminal; otherwise 0 is returned, and errno is set to indicate
	the error.

--------------------

### ttyname

Prototype:
```C
#include <unistd.h>

char *ttyname(int fd);
```
Description:

	The function ttyname() returns a pointer to the null-terminated
	pathname of the terminal device that is open on the file
	descriptor fd, or NULL on error (for example, if fd is not
	connected to a terminal). The return value may point to static
	data, possibly overwritten by the next call.

Return:

	The function ttyname() returns a pointer to a pathname on
	success. On error, NULL is returned, and errno is set to
	indicate the error.

--------------------

### ttyslot

Prototype:
```C
#include <unistd.h>

int ttyslot(void);
```
Description:

	The legacy function ttyslot() returns the index of the current
	user's entry in some file.
	https://man7.org/linux/man-pages/man3/ttyslot.3.html

Return:

	If successful, this function returns the slot number (>=1)
	On error returns <1

--------------------

### ioctl

Prototype:
```C
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```
Description:

	The ioctl() system call manipulates the underlying device
	parameters of special files. In particular, many operating
	characteristics of character special files (e.g., terminals) may
	be controlled with ioctl() requests. The argument fd must be an
	open file descriptor.

	The second argument is a device-dependent request code. The
	third argument is an untyped pointer to memory. It's
	traditionally char *argp (from the days before void * was valid
	C), and will be so named for this discussion.

	An ioctl() request has encoded in it whether the argument is an
	in parameter or out parameter, and the size of the argument argp
	in bytes. Macros and defines used in specifying an ioctl()
	request are located in the file <sys/ioctl.h>. See NOTES.
	https://man7.org/linux/man-pages/man2/ioctl.2.html

Return:

	Usually, on success zero is returned. A few ioctl() requests use
	the return value as an output parameter and return a nonnegative
	value on success. On error, -1 is returned, and errno is set to
	indicate the error.

Example code:
	Configure the terminal to turn off canonical mode (where input is
	processed line by line) and enable raw mode (where input is processed
	immediately)
```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

void set_raw_mode(int fd) {
	struct termios term;

	// Get the current terminal attributes
	if (ioctl(fd, TCGETS, &term) == -1) {
		perror("ioctl TCGETS");
		exit(EXIT_FAILURE);
	}

	// Modify the terminal attributes for raw mode
	term.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo

	// Set the terminal attributes
	if (ioctl(fd, TCSETS, &term) == -1) {
		perror("ioctl TCSETS");
		exit(EXIT_FAILURE);
	}
}

int main() {
	// Set the terminal to raw mode for the current session
	set_raw_mode(STDIN_FILENO);

	// Your shell logic here

	return 0;
}
```

--------------------

### getenv

Prototype:
```C
#include <stdlib.h>

char *getenv(const char *name);
```
Description:

	The getenv() function searches the environment list to find the
	environment variable name, and returns a pointer to the
	corresponding value string.

Return:

	The getenv() function returns a pointer to the value in the
	environment, or NULL if there is no match.

--------------------

### tcsetattr

Prototype:
```C
#include <termios.h>

int tcsetattr(int fildes, int optional_actions,
				const struct termios *termios_p);
```
Description:

	The tcsetattr() function shall set the parameters associated with
	the terminal referred to by the open file descriptor fildes (an
	open file descriptor associated with a terminal) from the termios
	structure referenced by termios_p as follows:

	*  If optional_actions is TCSANOW, the change shall occur
		immediately.

	*  If optional_actions is TCSADRAIN, the change shall occur
		after all output written to fildes is transmitted. This
		function should be used when changing parameters that affect
		output.

	*  If optional_actions is TCSAFLUSH, the change shall occur
		after all output written to fildes is transmitted, and all
		input so far received but not read shall be discarded before
		the change is made.

	If the output baud rate stored in the termios structure pointed
	to by termios_p is the zero baud rate, B0, the modem control
	lines shall no longer be asserted. Normally, this shall
	disconnect the line.

	If the input baud rate stored in the termios structure pointed to
	by termios_p is 0, the input baud rate given to the hardware is
	the same as the output baud rate stored in the termios structure.

	The tcsetattr() function shall return successfully if it was able
	to perform any of the requested actions, even if some of the
	requested actions could not be performed. It shall set all the
	attributes that the implementation supports as requested and
	leave all the attributes not supported by the implementation
	unchanged. If no part of the request can be honored, it shall
	return -1 and set errno to [EINVAL]. If the input and output
	baud rates differ and are a combination that is not supported,
	neither baud rate shall be changed. A subsequent call to
	tcgetattr() shall return the actual state of the terminal device
	(reflecting both the changes made and not made in the previous
	tcsetattr() call). The tcsetattr() function shall not change the
	values found in the termios structure under any circumstances.

	The effect of tcsetattr() is undefined if the value of the
	termios structure pointed to by termios_p was not derived from
	the result of a call to tcgetattr() on fildes; an application
	should modify only fields and flags defined by this volume of
	POSIX.1‐2017 between the call to tcgetattr() and tcsetattr(),
	leaving all other fields and flags unmodified.

Return:

	Upon successful completion, 0 shall be returned. Otherwise, -1
	shall be returned and errno set to indicate the error.

--------------------

### tcgetattr

Prototype:

```C
#include <termios.h>

int tcgetattr(int fildes, struct termios *termios_p);
```

Description:

	The tcgetattr() function shall get the parameters associated with
	the terminal referred to by fildes and store them in the termios
	structure referenced by termios_p. The fildes argument is an
	open file descriptor associated with a terminal.

	The termios_p argument is a pointer to a termios structure.

	The tcgetattr() operation is allowed from any process.

	If the terminal device supports different input and output baud
	rates, the baud rates stored in the termios structure returned by
	tcgetattr() shall reflect the actual baud rates, even if they are
	equal. If differing baud rates are not supported, the rate
	returned as the output baud rate shall be the actual baud rate.
	If the terminal device does not support split baud rates, the
	input baud rate stored in the termios structure shall be the
	output rate (as one of the symbolic values).

Return:

	Upon successful completion, 0 shall be returned. Otherwise, -1
	shall be returned and errno set to indicate the error.

--------------------

### tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

Prototype:
```C
#include <curses.h>
#include <term.h>

extern char PC;
extern char * UP;
extern char * BC;
extern @NCURSES_OSPEED@ ospeed;

int tgetent(char *bp, const char *name);
int tgetflag(const char *id);
int tgetnum(const char *id);
char *tgetstr(const char *id, char **area);
char *tgoto(const char *cap, int col, int row);
int tputs(const char *str, int affcnt, int (*putc)(int));
```
Description:

	Functions to manipulate terminal

--------------------

### dup, dup2

Prototype:
```C
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Definition of O_* constants */
#include <unistd.h>

int dup3(int oldfd, int newfd, int flags);
```
Description:

	The dup() system call allocates a new file descriptor that refers to the same open file description as the descriptor oldfd. (For an explanation of open file descriptions, see open(2).) The new file descriptor number
	is guaranteed to be the lowest-numbered file descriptor that was unused in the calling process.

	After a successful return, the old and new file descriptors may be used interchangeably.

	dup2()
	The dup2() system call performs the same task as dup(), but instead of using the lowest-numbered unused file descriptor, it uses the file descriptor number specified in newfd. In other words, the file descriptor newfd is adjusted so that it now refers to the same open file description as oldfd.

Return:

	On success, these system calls return the new file descriptor. On error, -1 is returned, and errno is set to indicate the error.

--------------------

### execve

Prototype:
```C
#include <unistd.h>

int execve(const char *pathname, char *const _Nullable argv[],
			char *const _Nullable envp[]);
```

Description:

	execve() executes the program referred to by pathname. This causes the program that is currently being run by the calling process to be replaced with a new program, with newly initialized stack, heap, and (initialized and uninitialized) data segments.

Return:

	On success, execve() does not return, on error -1 is returned, and errno is set to indicate the error.

--------------------

### fork

Prototype:
```C
#include <unistd.h>

pid_t fork(void);
```

Description:

	fork() creates a new process by duplicating the calling process. The new process is referred to as the child process. The calling process is referred to as the parent process.

Return:

	On success, the PID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, no child process is created, and errno is set to indicate the error.

--------------------
