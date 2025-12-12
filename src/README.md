# MeCab-Ko

> ❔ The information here is legacy as it referred to a previous iteration of what [the current project](https://github.com/jacoblockett/mecab) is. It is nevertheless valuable in case anything needs to be updated in a future where I haven't looked at the code in forever.

This uses a patched version of [mecab-ko-msvc](https://github.com/Pusnow/mecab-ko-msvc), updating the source to make the program portable rather than rely on some default C-like drive path on Windows machines, specifically tested against x64 architecture. This is used as the primary source for the MeCab N-API.

## Building from source

> You'll need [x64 Native Tools Command Prompt for VS 2022](https://aka.ms/vs/stable/vs_BuildTools.exe) in order to compile the program from source. (Search your system first, do not just click the link on auto-pilot. It's likely that something you've installed in the past automatically installed this program.)

First you'll need to clone this repository. Basically, copy it to your own system. To clone this repo, you have two options:

1. Click [this link](https://github.com/jacoblockett/mecab-ko/archive/refs/heads/master.zip) to download the zip file and extract it into a folder called `mecab-ko`.

2. Using the [`git`](https://git-scm.com/install/) command, clone the repo into a folder called `mecab-ko`.

   - ```bash
     git clone https://github.com/jacoblockett/mecab-ko.git "path/to/mecab-ko"
     ```

> ❔ The `mecab-ko` path doesn't have to be the final location of your compiled program. This is just where we'll set up and compile the program from. Details will be provided later in the instructions on how you can move the program.

Next, you'll want to open up the x64 Native Tools Command Prompt for VS 2022 and cd (change directory) into the `mecab-ko`'s src directory (for these instructions, we'll assume the repo clone lives at `C:/mecab-ko`):

```bash
**********************************************************************
** Visual Studio 2022 Developer Command Prompt v17.14.13
** Copyright (c) 2025 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'

C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools>cd "C:/mecab-ko/src"
```

Once there, do a quick "clean" command just to ensure your environment isn't stale:

```bash
C:\mecab-ko\src>nmake -f Makefile.msvc clean
```

Then, compile the program:

```bash
C:\mecab-ko\src>nmake -f Makefile.msvc
```

If all goes well, you shouldn't see any fatal errors in the terminal output, and instead you should see something like "Finished generating code". This code compiled into the `bin` directory on the root `mecab-ko` directory. Let's cd into there:

```bash
C:\mecab-ko\src>cd ../bin
```

Next up, we'll need to compile the dictionary MeCab will use for understanding the Korean language. To do so, we'll use one of the executables we just created with the `nmake` command called `mecab-dict-index.exe`. First, let's create a directory called `/dic`:

```bash
C:\mecab-ko\bin>mkdir dic
```

Then, we'll need to download and extract the dictionary files into that directory. You can find an uncompiled version of the dictionary by going to the [releases page](https://github.com/jacoblockett/mecab-ko/releases) of the repo under the most recent released version, titled `dictionary-uncompiled.zip`. When extracting, ensure the _contents_ of the zip file are copied into the `/dic` directory, not the full folder. In other words, you should **not** see `C:\mecab-ko\bin\dic\dictionary-compiled`, but instead `C:\mecab-ko\bin\dic\` should have a bunch of files that you extracted from the zip folder. If you do see the former, just copy its contents into the `/dic` directory.

> 💡 You can skip the following compilation step and download and extract the precompiled version of the dictionary, titled `dictionary-compiled.zip`, if you prefer.

Now, we're ready to compile. Run the following:

```bash
C:\mecab-ko\bin>mecab-dict-index.exe -d ./dic -o ./dic -f UTF-8 -t UTF-8
```

Again, if all goes well, you should see a confirmation term like "done!"

Finally, we need to make a file called `mecabrc` (with no extension). This is a kind of environment file that `mecab.exe` will look for when it wants to know where the dictionary is located.

```bash
C:\mecab-ko\bin>echo dicdir = ./dic>mecabrc
```

And that's it! Your compilation was hopefully successful and now you can verify it by doing something like the following:

```bash
C:\mecab-ko\bin>mecab.exe
```

Once running, the program will wait for additional input. Type in some Korean and you'll see some output like this:

```bash
C:\mecab-ko\bin>mecab.exe
아버지가방에들어가신다
아버지  NNG,*,F,아버지,*,*,*,*
가      JKS,*,F,가,*,*,*,*
방      NNG,장소,T,방,*,*,*,*
에      JKB,*,F,에,*,*,*,*
들어가  VV,*,F,들어가,*,*,*,*
신다    EP+EC,*,F,신다,Inflect,EP,EC,시/EP/*+ㄴ다/EC/*
EOS
```

If you happen to see output like:

```bash
C:\mecab-ko\bin>mecab.exe
아버지가방에들어가신다
???????????     SY,*,*,*,*,*,*,*
EOS
```

Press ctrl+c to cancel out of the program, then run

```bash
chcp 65001
```

This will switch your current command prompt environment to use UTF-8 encoding. You could also use the program in the following way:

```bash
C:\mecab-ko\bin>mecab.exe input.txt -o output.txt
```

You provide the text you want to parse in `input.txt`, and mecab will place the parsed result into `output.txt`. These files can exist anywhere you want and be named anything you want. Just make sure you provide the correct path to them.

Now, if you want to use this newly compiled version somewhere else on your system, you'll want to copy the bin folder in order for everything to work. Copy `/bin` anywhere and run `mecab.exe` from there and it should just work. To be more accurate, you only need:

- `/dic`
- `libmecab.dll`
- `mecab.exe`
- `mecabrc`

So long as those are all in the same directory when `mecab.exe` is called, everything should work.
