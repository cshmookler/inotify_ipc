# **inotify_ipc**

A socketless inter-process communication library for Linux.  Uses inotify (https://en.wikipedia.org/wiki/Inotify) to exchange messages between programs.

## Build from Source

### 1.&nbsp; Install a C++ compiler, Meson, and Git.

#### Linux (Debian/Ubuntu):

```bash
sudo apt install build-essential meson ninja git
```

#### Linux (Arch):

```bash
sudo pacman -S base-devel meson git
```

### 2.&nbsp; Clone this project.

```
git clone https://github.com/cshmookler/inotify_ipc.git
cd inotify_ipc
```

### 3.&nbsp; Build this project from source.

```
meson setup build
cd build
ninja
```

### 4.&nbsp; (Optional) Install this project globally.

```
meson install
```

## **TODO**

- [X] Add a method for sending messages.
- [X] Add a method for receiving messages.
- [ ] Add more tests.
- [X] Create an example involving two completely separate programs.
