 Quick WASM Tutorial
## Overview
WebAssembly or WASM is loosely used to refer to three things:
- A *model*  of a machine.
- A *programming langauge*.  There are two languages this may refer:
	- The machine code of a `.wasm` binary file i.e. a **WASM module**.
	- The human-readable *WebAssembly Text* format or *WAT* in a `.wat` file.
- The *implementation* of the model and language i.e. the WASM runtime.

In the browser, the WASM runtime is a *separate* runtime from the JavaScript runtime.  A JavaScript runtime can load WASM programs ( WASM modules) into a WASM runtime and then make calls to this runtime (via wrapper functions or through an API object called the **WASM instance**).

WASM modules can be created by compiling WAT , C/C++, or RUST programs into a WASM binary.

This tutorial will 
## Set-Up
The main tool we'll use is **Emscripten**: a compiler tool-chain for creating WASM binaries.  After setting up Emscripten, we can use the **Emscripten Compiler Frontend** or **emcc** to compile C or C++ programs into WASM binary files.

We'll also use **Python** and **Node.js** for running stuff.  We'll use something called **wabt** to explore WASM binary files.
### Installation for Emscripten
For some Linux distributions, you can install Emscripten through the package manager (e.g. `sudo apt install emsdk`).   However, it is recommended to build Emscripten from the official GitHub repository for the latest version.

(Note: I tried using the Emscripten version from the package amanger and couldn't get it to work with node.js, so I suggest from experience to build from source).

The following are the steps are for building from source.

Dependencies:
- [ ] Python 3.6+
- [ ] Git

Installation Steps:
1. From the CommandLine, go to where you want to install Emscripten.
	- For Linux, `~/.local/lib/` might be a good place.
2. Clone the Emscripten GitHub repository:
	- `$ git clone https://github.com/emscripten-core/emsdk.git`
3.  Change into the cloned repository (and optionally update):
	- `$ cd emsdk`
	- `$ git pull`
4. Install Emscripten:
	- `$ emsdk install latest`
5. Add the Emscripten toolkit into your current shell's path:
	- For *Windows* this is `$ activate .\emsdk_env.bat`
	- For *Linux* this is `$ source ./emsdk_env.sh`
	- You need to do this for every new shell session.  To make a more permanent:
		- On Linux, you can add the above to `.bashrc`
		- On Windows, you need to manually add `.emsdk_env` to the path variable and some other tweaks (I haven't actually tested this).
### Quick Test of Empscripten
As a quick test, we'll compile the following C file into a WASM module file.
``` C
// hello_world.c
#include <stdio.h>

int main() {
	printf("Hello, World!\n");
	return 0;
}
```

From the command-line, in some folder with the above C file:
``` Shell
$ emcc hello_world.c
```

We should now have two new files in the folder:
- `a.out.wasm` : this is the actual WASM module.  It's machine code, so opening it in a text-editor will assault your eyes.
- `a.out.js` : a JS script that just runs the WASM module.  You embed this into a web-page or run it on a web-server.
### Run WASM in the Browser
Let's make a simple web-page with the JS script, called `index.html`, in the same folder as the generated files:
``` HTML
<!DOCTYPE html>
<html>
	<head>
		<title>Hello, World!</title>
	</head>

	<body>
		<script src="a.out.js"></script>
	</body>
</html>
```

This needs to be hosted on a web-server (since JS can't access the local filesystem directly).  We can do this simply with Python; from the same folder:
``` Shell
$ python -m http.server
```

If you open the generated link in a browser and open the inspector (F12 or through the right-click/context-menu), you should see the printout `Hello, World!` in the console.

> [!note]- EMCC Auto-generate Web-Page
> We can automatically have *emcc* produce a html file with our `.js` script embedded:
> ``` Shell
> $ emcc -o output.html
> ```
> We won't use this since we don't really need it.  I think it would just complicate things.

### Run WASM in Node.js
Alternatively, we can run the `.js` script in a standalone JavaScript runtime environment like **Node.js**.

For our `.js` script:

``` Shell
$ node a.out.js
```

This will output `Hello, World!` onto the command-line, just like in a browser console.

### Output Files
> [!Warning] Section Needs Updating
> I don't think this section is entirely correct, based on further usage of `emcc`.  The official Emscripten documentation is rather sparse, too, on details, so I'll need to come back here after further investigation.  The problem is that `emcc` sometimes produces a standalone module when I don't tell it to; I think it has to do with `stdio.h`.

You can explictly signfiy the name of the output file *emcc* should produce.  The default (without arguments) is the same as:
``` Shell
$ emcc myProgram.c -o a.out.js
```

A WASM module (`.wasm` file) may be of two variants: *regular* or *standalone*.  A standalone WASM module is intended to run outside of the browser environment and therefore compiled to *WASI specifications*  (a superset of WASM specifications).  Trying to run a standalone WASM in the browser results in a *type error*.

Depending on the type of output file specified in the argument, *emcc* will automatically produce a regular or standalone WASM module, and additional files:
- `.js` : the default options produces a `.wasm` module and `.js` file that embeds the WASM module.  Depending on the contents of the input file, `.wasm` might be a standalone module or regular module (this was my experience, but it is not documented in the *emcc* manual).
- `.wasm` : only produces a standalone `.wasm` module.
- `.html` : produces a regular `.wasm` module, a `.js` file, and a `.html` file.

You can explictly command *emcc* to produce a standalone WASM module with the `-s STANDALONE` argument.

You can differentiate names for the support `.js` file and `.wasm` module too:
``` Shell
$ emcc myProgram.c -o myScript.js ; myModule.wasm
```

### Viewing WASM Code as WAT
The generated `.wasm` file is in machine code, and therefore not human readable.  However, there is a suite of tools called **WebAssembly Binary Toolkit** or **wabt** that provides applications for exploring WASM binaries, including an application we can use to convert a `.wasm.` binary into a readable `.wat` file.  On Linux, *wabt* is distributed by most package managers; or you can build from source [here](https://github.com/WebAssembly/wabt?tab=readme-ov-file).

To generate the `.wat` file (that we'll call `a.out.wat`) of our `a.out.wasm` binary:

``` Shell
$ wasm2wat a.out.wasm -o a.out.wat
```

View the `.wat` file in a text editor or in `less` or something.  Note the large size of the `.wat` program; that's because we included the standard library `stdio.h` in our original C file.

There is an online version of this tool at: 
> [https://webassembly.github.io/wabt/demo/wasm2wat/]()

Alternatively, you view the WASM binary through the browser inspector on Chromium or FireFox; the binary is automatically converted to the WAT format.  In Chromium, view this under *Sources* in the inspector.  In FireFox, this is under *Debugger*.



## 1. Loading WASM
So far, we learned to write a simple C program and compile it to a WASM binary.  Then a JS script in a webpage immediately runs the WASM program when the webpage is opened.

A more advanced scenario is that we use C or C++ to define some functions and have them accessible via WASM to JavaScript in the browser (in other words, export the functions).  Since these functions are in WASM, they run more efficiently than equivalent functions in pure JavaScript.  These functions can be run "whenever".

### Exporting Functions to CCALL
As a starting example, a C program with the function `addNums()` we want to export:
``` C
// maths.c
#include <stdio.h>

int addNums(int a, int b) {
	return a + b;
}

int main() {
	printf("Sum: %d", addNums(10, 13));
	return 0;
}
```


To begin, we first need to include a header file `emscripten.h` which gives us access to certain macros and directives.  Next we then prefix the functions we want to export with the keyword `EMSCRIPTEN_KEEPALIVE` (learn more [here](https://emscripten.org/docs/api_reference/emscripten.h.html#compiling)).
``` C
#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int addNums(int a, int b) {
	return a + b;
}

EMSCRIPTEN_KEEPALIVE
int main() {
	printf("Sum: %d", addNums(10, 13));
	return 0;
}
```

Note, the directive might be `<emscripten/emscripten.h>`, depending on your installation.  You may also need to use the `extern` keyword:
``` C
	extern EMSCRIPTEN_KEEPALIVE int main() { ... }
```

When compiling, we now need to include additional arguments to *emcc*:
``` Shell
$ emcc maths.c -o maths.js -s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall]
```

Explanations of the arguments:
- `-s NO_EXIT_RUNTIME=1` : the WASM program will not end after the `main()` function finishes (so the exported functions remain accessible).
- `-s EXPORTED_RUNTIME_METHODS=[ccall]` : exported functions will be accessed by JavaScript through the `ccall()` function.
	- That is, all our exported functions can be used by calling `ccall()` in JavaScript via specific arguments.
	- The compiler will now look for functions labeled `EMSCRIPTEN_KEEPALIVE` to export.

We should now have two new files:
- `maths.wasm`
- `maths.js`

We have the web-page with a button that will call the `addNums()` function whenever it is pressed:
``` html
<!DOCTYPE html>
<html>
	<head>
		<title>Math with WASM</title>
		<script src="maths.js"></script>
	</head>
	<body>
		<script>
			function run_wasm() {
				var result = Module.ccall("main", "number", null, null);
				console.log(result);
			}
		</script>
		<button onclick="run_wasm()">Add!</button>
	</body>
</html>
```

An explanation:
- We load `maths.js` with the script in the header.
- `maths.js` provides an object called `Module` with the method `ccall()`.  
- We pass specific arguments to `ccall()` to call a specific exported function.  The parameters to `ccall()` are:
	1. The function name - here we are calling the `main()` function.
	2. The return type - `main()` returns a `number` type (originally an `int` in C, in JavaScript this is `number`).
	3. List of argument types - `main()` does not take arguments, so `null`.
	4. List of argument values - `main()` does not take arguments, so `null`.

If we open this web-page, the console immediately shows `Sum: 23`.  This is the `main()` function running first from the script in the header.  Pressing the button outputs another `Sum: 23` (the side-effect of `main()`), and then `0` (the side-effect of `run_wasm()`.

> [!Warning]- Browser Error
> Make sure `printf()` in your original C code ends in a newline (`\n`) otherwise you may get a `Buffer not flushed` error.

WASM itself can't interact with the DOM.  JavaScript passes along values from the DOM to exported WASM functions.  We change our web-page to include input-forms where users can enter two numbers to add together.

``` html
<!DOCTYPE html>
<html>
	<head>
		<title>Math with WASM</title>
		<script src="maths.js"></script>
	</head>
	<body>
		<script>
			function run_wasm() {
				var a = document.querySelector("#a").value;
				var b = document.querySelector("#b").value;
				var result = Module.ccall(
					"addNums", 
					"number", 
					["number", "number"],
					[a, b]);
				console.log(result);
			}
		</script>
		
		<input type="number" id="a">
		<input type="number" id="b">
		<button onclick="run_wasm()">Add!</button>
	</body>
</html>
```

As a reminder, thew new `Module.ccall()` indicates:
- `"addNums"` : exported function we are calling is `addNums()`.
- `"number"` : return type is `number`.
- `["number", "number"]` : there are two arguments of type `number` and `number`.
- `[a, b]` : the actual arguments are `a` and `b`, initialized in the preceding lines.

If we now go to this web-page, we can input two numbers and press the button to see the sum appear in the console.  However, you'll note that `main()` also runs whenever we make a call to `addNums()`; this is because `ccall()` basically *reloads* the C program (compiled to WASM) and runs the exported function with the provided arguments.

So using `ccall()` has two quirks:
1. Since `ccall()` returns the return-value of the exported function, we need to call `ccall()` again if we want to call the same exported function.
2. The `main()` function runs whenever we call `ccall()`.

The reason for the above is that we are reloading the WASM module everytime we make a call to `ccall()` ^[I need to look into this more].
### Exporting Functions to CWRAP
If we just need to load a WASM module and exported function once, `ccall` is okay.  If we need to repeatedly make calls to an exported function, then `ccall` is tedious and expensive.  It would be better if we could just load a WASM module, and then have JavaScript functions as wrappers around to the exported functions.  This is what `cwrap` does.

To export functions to `ccall()` and `cwrap()`, we slightly change the arguments to *emcc*:
``` Shell
$ emcc maths.c -o maths.js -s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall,cwrap]
```
Alternatively, if we just need `cwrap`, we can exclude `ccall`.

We change our web-page to do so as previously, but with `cwrap()` instead of `ccall()`.

``` html
<!DOCTYPE html>
<html>
	<head>
		<title>Math with WASM</title>
		<script src="maths.js"></script>
	</head>
	<body>
		<script>
			function run_wasm() {
				var addNums = Module.cwrap(
					"addNums",
					"number",
					["number", "number"]);
					
				var a = document.querySelector("#a").value;
				var b = document.querySelector("#b").value;
				var result = addNums(a, b);
				console.log(result);
			}
		</script>
		
		<input type="number" id="a">
		<input type="number" id="b">
		<button onclick="run_wasm()">Add!</button>
	</body>
</html>
```

Note that:
- `ccall()` returns the return-value of the exported function.
- `cwrap()` returns the exported function itself.

So the variable `addNums` is a function pointer to the exported function `addNums()`.

Note that the `main()` function now only runs once, when the web-page first loads, rather than for every call of `addNums()`.

## 2. Streaming WASM
 We can run WASM binaries without needing to embed the JS file that *emcc* outputs.  Instead, we'll just fetch the WASM binary whenever we need to call an exported function.  The advantages of this approach:
 - Easier compilation: we don't need additional commandline arguments.
 - Less resource-intensive: we just need the WASM binary.  No need to embed a JS script.

As a simple example, let's say we have the following C program:
``` C
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int main() {return 0;}

EMSCRIPTEN_KEEPALIVE
int addNums(int a, int b) {
	return a + b;
}

EMSCRIPTEN_KEEPALIVE
int subNums(int a, int b) {
	return a - b;
}
```

We simply compile as:
``` Shell
$ emcc maths.c -o maths.js
```

Our web-page looks like:
``` html
<!DOCTYPE html>
<html>
    <head>
        <title>Streaming WASM</title>
    </head>
    <body>
        <script>
            function run_wasm() {
                WebAssembly.instantiateStreaming(
                    fetch("maths.wasm"), {}
                ).then(results => {
                    var a = document.querySelector("#a").value;
                    var b = document.querySelector("#b").value;
                    var sum = results.instance.exports.addNums(a, b);

                    document.querySelector("#out").innerHTML += `${a} + ${b} = ${sum}<br/>`;
                });
            }
        </script>

        <input type="number" id="a">
        <input type="number" id="b">
        <button onclick="run_wasm()">Plus</button>
        <p id="out"></p>
    </body>
</html>
```

Some notes:
- We no longer load the `.js` output in the header or anywhere.
- The browser environment already has an object called `WebAssembly`.  The method `WebAssembly.instantiateStreaming()` is an asynchronous function that returns a module of our WASM program (wrapped in a promise object).  It takes two arguments:
	1. The WASM binary via the Fetch API.
	2. The *imports* as an object.  For now this is empty (given as `{}`).
- The exported functions are found as methods of the object `results.instance.exports`.  The exact name of the function must be given (`addNums` here).  Later we'll learn about name mangling for C++/WASM programs.

Similar to what we did with `cwrap`, we don't have to fetch the WASM binary for every call.  Instead, we can store the fetched WASM module in a variable.  We also can see how easy it is to also call the function `subNums()`.

``` html
<!DOCTYPE html>
<html>
    <head>
        <title>Streaming WASM</title>
    </head>
    <body>
        <script>
            var exports;
            WebAssembly.instantiateStreaming(
                fetch("maths.wasm"), {}
            ).then(results => exports = results.instance.exports);

            function run_add() {
                var a = document.querySelector("#a").value;
                var b = document.querySelector("#b").value;
                var sum = exports.addNums(a, b);

                document.querySelector("#out").innerHTML += `${a} + ${b} = ${sum}<br/>`;
            };
            
            function run_sub() {
                var a = document.querySelector("#a").value;
                var b = document.querySelector("#b").value;
                var sum = exports.subNums(a, b);

                document.querySelector("#out").innerHTML += `${a} - ${b} = ${sum}<br/>`;
            };
        </script>

        <input type="number" id="a">
        <input type="number" id="b">
        <button onclick="run_add()">Plus</button>
        <button onclick="run_sub()">Minus</button>
        <p id="out"></p>
    </body>
</html>
```

Now the variable `exports` holds a *WASM instance* object  

> [!Warning]- Libraries and WASI
> There is an important difference between exporting functions and streaming: without the wrapping JS script, libraries that make use of standard I/O or file manipulation won't work.  The browser gets all the exported functions at once through the asynchronous `instatiateStreaming()`; if any function makes use of such libraries, the call will fail (return a rejected promise).  For example, if the `main()` function in our C code was:
> ``` C
> int main() { printf("Hey\n") };
> ```
> Our web-page throws an error when attempting stream the WASM binary.  Note the specific error is that `instatiateStreaming()` returns a `wasi_snapshot_preview.wasm` file; such libraries need to make use of WASI, which we will learn later.

## 3. Compiling Multiple Files
Compiling Multiple C files to a single WASM binary is no issue; *emcc* works similar to *gcc* or any other C compiler in that regards.

## 4. Arrays and Memory
The WASM runtime, in which WASM modules are executed, is a WASM virtual machine which has **linear memory**.  The heap of C/C++ programs when compiled to WASM are represented in this linear memory.  If we write programs that use arrays or pointers, we need access to this memory.

This linear memory is part of the WASM runtime, and not directly accessible to the JavaScript runtime.  To access this linear memory from JavaScript, we need to use an **array buffer**.  Writing to this array is akin to writing to the WASM virtual machine's memory.  

> [!Note]- C/C++ Memory Management
> C/C++ has more hands-on memory management through `malloc()` and gang, but we'll get to those later.
### Getting a Memory Buffer
We create a WASM *memory object* that holds the array buffer.

``` JavaScript
var protoMemory = WebAssembly.Memory( {initial: 256, maximum: 512} );
```

The argument to the constructor is an object with the initial and maximum number of *pages* of the buffer.  In WASM, page sizes are fixed to 64 KiB.  So in the above example the buffer starts as 16 MB (256 pages) and can grow to 33 MB (512 pages).  For more details, see [here](https://webassembly.org/getting-started/js-api/).

We link this array buffer to a WASM module when we load it.  Via streaming, 

### Passing an Array to a Function
As an example, a simple C function that sums the numbers in an array of integers.  We pass a pointer to the array `*arr` and the size of the array `n`.

``` C
#include <emscripten.h>

int main() { return 0; }

EMSCRIPTEN_KEEPALIVE
int accumulate(int *arr, int n) {
	int sum = 0;
	while (n > 0) {
		sum += arr[--n];
	}
	return sum;
}
```

We don't actually need to keep `main()` alive; it serves simply as the entry point for the WASM runtime.

We'll compile this with *emcc* and stream the WASM to our webpage.

``` html
$ emcc memory.c -o memory.js
```

> [!warning]- Use *emcc* to create a HTML file
> Make the output for the compilation a `.html` file.  This forces *emcc* to compile a regular WASM module, rather than a standalone module.  For some reason, the argument `-o memory.js` creates a standalone `.wasm` file, which is *not* what we want; this will create a type-error when we attempt to stream in into the browser.

``` HTML
!DOCTYPE html>
<html>
    <head>
        <title>Memory in WASM</title>
    </head>

    <body>
        <p>Memory in WASM</p>
        <script>
            var protoMemory = new WebAssembly.Memory({
                initial: 256,
                maximum: 512,
            });
            var memory;
            var exports;

            WebAssembly.instantiateStreaming(fetch("memory.wasm"), 
                {
                    js : { mem: memory}
                }
            )
                .then(results => {
                    exports = results.instance.exports;
                    memory = results.instance.exports.memory;
                });

            function run_wasm() {
                var arr = new Uint32Array(memory.buffer);
                for (var i = 0; i < 10; i++) {
                    arr[i] = i * 2;
                }

                var sum = exports.accumulate(0, 10);

                document.querySelector("#return")
                    .innerHTML += `${sum}<br>`;
            }
        </script>

        <input type="number" id="a">
        <button onclick="run_wasm()">Accumulate</button>
        <p id="return"></p>
    </body>

</html>
```
### Returning an Array from a Function
We can have a function return an array, too.  In C, an array is a pointer.  So in our function..
``` C
// memory.c
#include <emscripten.h>

int main() {return 0;}

EMSCRIPTEN_KEEPALIVE
int accumulate(int *arr, int n) {
    int sum = 0;
    while (n > 0) {
        sum += arr[--n];
    }
    return sum;
}

EMSCRIPTEN_KEEPALIVE
const char *getString() {
    return "Hello, World!";
}
```

We then extend our web-page to print a String to the console.

``` HTML
!DOCTYPE html>
<html>
    <head>
        <title>Memory in WASM</title>
    </head>

    <body>
        <p>Memory in WASM</p>
        <script>
            var protoMemory = new WebAssembly.Memory({
                initial: 256,
                maximum: 512,
            });
            var memory;
            var exports;

            WebAssembly.instantiateStreaming(fetch("memory.wasm"), 
                {
                    js : { mem: memory}
                }
            )
                .then(results => {
                    exports = results.instance.exports;
                    memory = results.instance.exports.memory;
                });

            function run_wasm() {
                var arr = new Uint32Array(memory.buffer);
                for (var i = 0; i < 10; i++) {
                    arr[i] = i * 2;
                }

                var sum = exports.accumulate(0, 10);

                document.querySelector("#return")
                    .innerHTML += `${sum}<br>`;
            }

            function get_string() {
                var ptr = exports.getString();
                var got_string = new Uint8Array(memory.buffer, ptr);

                var str = new TextDecoder("utf8").decode(
                    got_string.slice(0, 13)
                );
                console.log(str);
            }
        </script>

        <input type="number" id="a">
        <button onclick="run_wasm()">Accumulate</button>
        <button onclick="get_string()">Get String</button>
        <p id="return"></p>
    </body>

</html>
```

## 5. Importing Functions from JavaScript
So far, we learned to export functions from WASM to JavaScript.  In other words, we can use WASM functions from JavaScript.  We can also go the other way around: have the WASM module use functions from JavaScript.

In our C file, the JavaScript functions we want to import are declared as `extern` functions.

> [!Warning]- Compilation Warning
> Compiling now will produce a warning because the *emcc* linker can't find where these functions are defined, but that's okay since we are going to provide/import these functions when the WASM module is instantiated (loaded into the browser with `instantiateStreaming()`).


## 6. Import Object
In the previous two sections, we made use of the **import object**.  This is a JavaScript object whose attributes signify how the WASM module should be instantiated (loaded into the WASM runtime).  The two things we specified were:
- the WASM machine's memory
- imported functions

