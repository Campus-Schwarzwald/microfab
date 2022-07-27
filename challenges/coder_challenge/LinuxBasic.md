# Basic Linux commands

## File Hierarchy Standard (FHS)
| Path   | Content                             |
| ------ | ----------------------------------- |
| `/bin`   | Binaries (User)                     |
| `/boot ` | Static boot loader files            |
| `/etc`   | Host specific configs               |
| `/lib `  | Shared libraries and kernel modules |
| `/sbin ` | Binaries (System/root)              |
| `/var`   | Varying files (e.g. Logs)           |
| `/usr `  | 3rd party software                  |
| `/proc`  | Pseudo file system                  |
| `/sys `  | Pseudo file system                  |
| `/mnt `  | Mountpoint for internal drives      |
| `/media` | Mountpoint for external drives      |
| `/home`  | User homes                          |
| `/run`   | PID files of running processes      |

| Command | Param            | Description                                       |
| ------- | ---------------- |---------------------------------------------------|
| `cd`    | `-`              | Navigate to last dir                              |
|         | `~`              | Navigate to home                                  |
|         | `~username`      | Navigate to home of specified user                |
| `pwd`   |                  | Print working dir                                 |
| `ls`    |                  | Print dir content                                 |
|         | `-l`             | Format as list                                    |
|         | `-a`             | Show hidden items (`-A` without `.` and `..`)     |
|         | `-r`             | Invert order                                      |
|         | `-R`             | Recurse                                           |
|         | `-S`             | Sort by size                                      |
|         | `-t`             | Sort by date modified                             |
| `mkdir` | `-p`             | Create dir with parents                           |
| `cp`    | `-r`             | Copy dir                                          |
| `rmdir` | `-p`             | Remove dir and empty parents                      |
| `rm`    | `-rf`            | Remove dir recursively, `-f` without confirmation |
| `mv`    |                  | Move recursively  or rename files                 |
| `find`  | `-iname pattern` | Search dir/file case-insensitive                  |
|         | `-mmin n`        | Last modified n minutes ago                       |
|         | `-mtime n`       | Last modified n days ago                          |
|         | `-regex pattern` | Path matches pattern                              |
|         | `-size n[kMG]`   | By file size (`-n` less than; `+n` greater than)  |
|         | `! searchparams` | Invert search                                     |

## File manipulation

| Command | Param                    | Description                                     |
|---------|--------------------------|-------------------------------------------------|
| `cat`   | `file`                   | Print content of file                           |
|         | `file_1 file_2 > file_3` | Combine `file_1` and `file_2` to `file_3`       |
|         | `file_1 >> file_2`       | Append `file_1` to `file_2`                     |
| `nano`  | `file`                   | Open the nano editor for a new or existing file |
| `touch` | `file`                   | Create a new empty `file`                         |

## System units

| Command      | Param                 | Description                                          |
|--------------|-----------------------|------------------------------------------------------|
| `systemctl`  | `enable unitname`     | Enabling the `unitname`                              |
|              | `start unitname`      | Starting `unitname`                                  |
|              | `stop unitname`       | Stopping `unitname`                                  |
|              | `restart unitname`    | Restarting `unitname`, if not started start          |
|              | `status unitname`     | Status of the `unitname`                             |
|              | `daemon-reload`       | Load all config files from all units and restart all |
| `journalctl` | `-u service-name`     | Show logs for a service                              |
|              | `-n numer-of-logs -f` | Show the last `n` logs                                 |


## Remote access

| Command | Param                                                  | Description                                         |
|---------|--------------------------------------------------------|-----------------------------------------------------|
| `ssh`   | `user@hostname.local`                                  | connect via SSH for a defined `user` / `hostname`   |
|         | `user@ip`                                              | connect via SSH for a defined `user` / `IP`         |
| `scp`   | `user@hostname.local:remote-file-path local-file-path` | Copy a file from a remote host to the local machine |


## Others

| Command     | Param       | Description                                        |
|-------------|-------------|----------------------------------------------------|
| `ifconfig`  |             | show network adapters                              |
| `ping`      | `ip-adress` | Ping a specified IP adress                         |
