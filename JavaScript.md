# Asynchronous JavaScript
Recollecting my notes from *Eloquent JavaScript*, *JavaScript.info*, and other places.

> Both prominent JavaScript programming platforms—browsers and Node.js—make operations that might take a while asynchronous, rather than relying on threads. Since programming with threads is notoriously hard (understanding what a program does is much more difficult when it’s doing multiple things at once), this is generally considered a good thing.
>  -*Eloquent JavaScript*

A JavaScript program ..


## Promises
### Overview

The `Promise` is a class of objects similar to the `Result` type in Rust, in that it is a wrapper around a value.  Specifically, it is a wrapper around the result of an asynchronous function: the "promise" of a future value.  For now, it stands to simply know it is a wrapper around a value.

A `Promise` object can be of two types:
- **Resolved (or fulfilled) Promise:** meaning the object wraps around a valid value.  The object contains two attributes:
	1. `State: 'Fulfilled'` indicates the Promise is resolved.
	2. `Value: <some-value>` which is the actual value.
- **Failed (or rejected) Promise**: meaning the object wraps around an error (or invalid value).  The object attributes are:
	1. `State: 'rejected'` indicates the Promise is rejected.
	2. `Reason: <some-value>` indicating why the Promise failed.

For now think, think of these as `Result::Ok` and `Result::Err` in Rust.  

Here's an important part of Promise objects that many tutorials and books don't stress enough: **you can't unwrap the value inside the Promise object**.  There is no JavaScript equivalent of `.unwrap()` in Rust.  This is by design: the Promise object acts as a guard between the rest of the program and the asychronous aspects of the value it holds.  Only through the Promise object's methods can the rest of the program access the held value.  There are three major methods to access values:
- `.then()`
- `.catch()`
- `.finally()`
### Resolved Promise
We'll start with Promise objects that wrap around valid values.  To explicitly make a resolved Promise object, we can use the `.resolve()` class method:
```JavaScript
let fifteen = Promise.resolve(15)
```
Here, `fifteen` refers to a new Promise object with attributes:
- `<state>: 'fulfilled'`
- `<value>: 15`

The method to access the valid value is `.then()` where you pass a **callback function** as an argument.  The callback function takes the value in the Promise object as an argument.  The `.then()` method effectively asks the Promise object to apply the callback function to the value.  As such, this callback function is called the **resolve handler**.

The `.then()` method always returns *another Promise object*, no matter the `return` statement of the resolve handler function.  For example:

```JavaScript
let fifteen = Promise.resolve(15);
	// fifteen === Promise {<state>: 'fulfilled', <value>: 15}
let sixteen = fifteen.then(value => value + 1);
	// fifteen === Promise {<state>: 'fulfilled', <value>: 15}
	// sixteen === Promise {<state>: 'fulfilled', <value>: 16}
	// NOT sixteen === 16
```

For a resolve handler without a return, the new Promise object holds `undefined` in its value.  For example, the function `console.log()` being passed as the resolve handler only prints the value as a side-effect:

```JavaScript
let fifteen = Promise.resolve(15);
	// fifteen === Promise {<state>: 'fulfilled', <value>: 15}
let another = fifteen.then(console.log);
	// fifteen === Promise {<state>: 'fulfilled', <value>: 15}
	// another === Promise {<state>: 'fulfilled', <value>: undefined}
```

Because `.then()` returns another Promise object, we can chain `.then()` methods together, which mimics nesting callback functions within one another.

So far the design of Promise objects might seem contrived, but remember that they are meant be useful in an asynchronous context, like in reading files.

> [!Note]- "Unwrapping" the Value
> Though I said unwrapping is not really possible, here's a "trick" that exploits the closure of a callback function:
> ``` JavaScript
> 	var a; 				
> 	function aCallBack (value) {a = value;}
> 	let fifteen = Promise.resolve(15);
> 	fifteen.then(aCallBack);
> 	// a === 15
> ```
> This is generally not a good idea; 
> // TODO: elaborate

### Failed Promises
A Promise object can be of a "failed value" type, like `Result::Err` in Rust. 

To explicitly make a failed Promise, we can use the class method `.reject()` where the passed argument may be an `Error` object or a `String`, i.e. the result of a `throw`, indicating the reason for failure; this argument will be held as the value for the attribute `<reason>`.  

In normal usage (which we'll get to in the next section), a failed Promise object is created when an asynchronous function throws an error, which is then caught by the Promise constructor.  Again, this makes more sense later, as using explicit failed Promise objects doesn't make much sense without the larger context of asynchronous programming.

The method `.catch()` on a failed Promise acts similar to `.then()` on a resolved Promise: 
- A callback function is passed as an argument to `.catch()`, which is called *only* on a rejected Promise and returns a new Promise depending whether the callback returns or throws an error:
	- This callback function will take the `<reason>` value as its argument and will return a *new* fulfilled Promise with return value for `<value>`.  
	- If this callback function throws an error, then `.catch()` will return a *new* rejected Promise, with the error as the value of `<reason>`.  
- If `.catch()` is called on a resolved Promise, the original resolved Promise is simply returned.

```JavaScript
let rejectedPromise = Promise.reject('Explictly made to fail');
let resolvedPromise = Promise.resolve('A Valid Value');

rejectedPromise.catch(console.log);
	// outputs 'Explictly made to fail'
	// returns new Promise {<state>: 'fulfilled', <value>: undefined}
resolvedPromise.catch(console.log);
	// outputs nothing, returns resolvedPromise again

rejectedPromise.catch(value => {return value + ' but caught :-)'});
	// returns new Promise {<state>: 'fulfilled', <value>: "Explicitly made to fail but caught :-)"}

rejectedPromise.catch(() => {throw "Some error"});
	// returns new Promise {<state>: 'rejected', <reason>: "Some error"}
```

Calling `.then()` on a failed Promise object will simply return the same failed Promise object again (mirroring `.catch()` on a resolved Promise object).
### Shortcut Methods
The method `.then()` can optionally take two arguments: `.then(<resolve handler>, <failure handler>)`.  The first argument is the callback function if `.then()` is called on a resolved Promise object and the second argument is the callback function if it is called on a failed Promise object.  This is equivalent to chaining `.then(<resolve handler>).catch(<failure handler>)`.

The method `.finally()` is used if you want to run a callback function in either case a promise resolves or fails.  That is, you don't care if the promise resolves or fails, you want to run the same callback function in either case.  So if you want to run `sayHello()` after a `mypromise` resolves or fails, instead of writing out:
``` JS
mypromise.then(sayHello).catch(sayHello);
```

I can instead, as a shorthand, write:
``` JS
mypromise.finally(sayHello);
```

It returns back the same promise.  This is particularly useful at the end a method chain on a promise object.  
### Promise Constructor
The methods `.resolve()` and `.reject()` immediately create a resolved or rejected Promise object.  More generally, a `Promise` object is an object that will be a resolved *or* rejected Promise object ..

We can create such a general Promise object with the `Promise` constructor:

``` JavaScript
generalPromise = new Promise(specialFunction);
```

The `generalPromise` will be either a resolved Promise or a rejected Promise at a later time, depending on how `specialFunction` executes.  The definition of `specialFunction` must follow a specified syntax; namely, the constructor will pass two functions as arguments to `specialFunctions`:
- `resolve`, which is effectively the `.resolve()` method we saw earlier.
- `reject`, which is effectively the `.reject()` method we saw earlier.

For example: 

``` JavaScript
function mySpecialFunction (resolve, reject) {
	let randomNumber = Math.ceil(Math.random() * 5));

	if (randomNumber < 3) {
		resolve(randomNumber);
	} else {
		reject('An error occurred');
	}
}

let myPromise = new Promise( mySpecialFunction );
```

The `mySpecialFunction()` tells the constructor to create a resolved Promise if `randomNumber < 3` and a rejected Promise otherwise.  Here, `mySpecialFunction()` is quite simple, but you might begin to imagine if `mySpecialFunction()` were an asynchronous function.

> [!Warning]- Use Arrow Functions
> You'll often see the special function passed to the `Promise` constructor defined as an **arrow function**.  This is often because the Promise object is returned in a class function, where **lexical scoping** is needed resolve the keywork `this`.


### Example: WiFi Cracking
#### The Problem
With all this knowledge, the example from *Eloquent JavaScript* should know make a little more sense.

The scenario is this: we are trying to guess the passcode of a WiFi router.  We know the passcode is six-digits.  We are given an *asynchronous* API function `joinWifi()` that tries to join the WiFi network; it returns a promise object depending on the success.
- `joinWifi(<network-name>, <passcode>)`
	- `<network-name>` : `string` : name of the network (not important in this example).
	- `<passcode>` : `string` : the WiFi passcode, assumed to be all six-digits (e.g. `123456`).
- Returns promise object: resolved if connection successful, failed otherwise.

The WiFi router has a quirk that we can exploit: if we send a partial code (for example, only three-digits `123`)...
- ...and the passcode digits are correct so far, the router will wait for more digits to be input.
- ...and the digits are incorrect, the API function immediately resolves to failure.

So if the actual passcode is `333333` and we send `joinWifi('my-network', '333')`, the function will just hang.

#### Solution
We can brute-force the password.  Start with a one-digit partial code: `0`.
1. Enter the partial passcode.
	1. If it immediately fails, the last digit was incorrect.  Increment the passcode and repeat.
	2. If after a set time the function does not resolve, the partial code must be correct so far.  Add another digit and repeat.
2. Repeat the previous steps until we get the full passcode.

So if the passcode is `333333`, we would be entering:
1. `0`
2. `1`
3. `2`
4. `3`
5. `30`
6. `31`
7. `32`
8. `33`
9. `330`
10. ... and so on, until we hit `333333`.

Our first step is to create a time-out wrapper function around `joinWifi()`.    The function needs to take `joinWifi()` as an argument, so we can modify the passcode.

We will use the built-in asynchronous JavaScript function `setTimeout()`:
- `setTimeout(<my-function>, <time>)`
	- After `<time>` milliseconds, our `<my-function>` function will fire.

So our time-out wrapper function looks like:
``` JavaScript
function withTimeout(apromise, time) {
	return new Promise((resolve, reject) => {
		apromise.then(resolve, reject);
		setTimeout(() => reject("Timed out"), time)
	});
}
```

 The parameters:
 - `apromise` is a promise object (in our case, from `joinWifi()`).
 - `time` is how many milliseconds we want wait until we consider the partial code to have been correct.

The asynchronous actions by `apromise` will run at the same time as `setTimeout()`.
- If `apromise` fails before the timeout, then the passcode was incorrect.
- If the timeout occurs before `apromise` resolves or fails, then the entered partial passcode was correct and the router was waiting for more digits.
- If `apromise` resolves to success, then the entered full passcode was correct.

We can create a recursive function `nextDigit()` to guess the digits of the passcode.  The full code looks like:

``` JavaScript
function crackPassword(networkID) {
	function nextDigit(code, digit) {
		let newCode = code + digit;
		return withTimeout(joinWifi(networkID, newCode), 50)
				.then(() => newCode)
				.catch(failure => {
					if (failures == "Timed out") {
						return nextDigit(newCode, 0);
					} else if (digit < 9) {
						return nextDigit(code, digit + 1);
					} else {
						throw failure;
					}
				});	
	}
	return nextDigit("", 0);
}
```

The argument `code` is our partial code and `digit` is the next digit we try.

## Asynchronous Functions
### Overview
When writing asynchronous JavaScript programs, we'll mostly use asychronous functions that are provided to us, such as `setTimeout()` or something from *AJAX*, as we've done so far.  However, we can use these in a function defintion to make our own asynchronous function.

The example from the previous section highlights some places where some shortcuts would be useful.
1. It would be nice if we could just have functions automatically wrap return values in promise objects, instead of explicitly doing it ourselves.
2. The solution to our problem in the previous example was linear; despite `joinWifi()` being asynchronous, we waited for each returned promise to settle before moving on to the next iteration.  To achieve this, we made `nextDigit()` a recursive function.  It would be nice if we could do the same without the need for recursion.

JavaScript helps us out in both cases with the `async` and `await` keywords, which ..

### Async
The keyword `async` declares the function as *asynchronous*, and a JavaScript interpreter (or compiler) will handle it as such.  When placed in front of a function definition, the function automatically wrap the return values in a promise object.  So writing the below:
``` JavaScript
async function f() {
	return 1;
}
```

Is the same as writing:
``` JavaScript
function f() {
	return Promise.resolve(1);
}
```

If the function throws an error, the function returns a failed promise object.  We can also explicitly return promise objects:
``` JavaScript
async function f() {
	return Promise.reject("Failed async function");
}
```
### Await
The keyword `async` declared before a function means we can use the keyword `await` within its definition.

The keyword `await` in front of a promise object means to wait for the promise object to settle before continuing the rest of the program execution.  This would be similar to "wait" or "join" in languages with threading or other asynchronous programming capabilities.  As *Eloquent JavaScript* puts it, this allows you to write "pseudo-synchronous" definitions for functions.

We can rewrite the `crackPassword()` password from the previous example to utilize `await`.  

``` JavaScript
async function crackPasscode(networkID) {
	for (let code = "";;) {
		for (let digit = 0;; digit++) {
			let newCode = code + digit;
			try {
				await withTimeout(joinWifi(networkId, newCode), 50);
				return newCode;
			} catch (failure) {
				if (failure == "Timed out") {
					code = newCode;
					break;
				} else if (digit == 9) {
					throw failure;
				}
			}
		}
	}
}
```

Hopefully this makes the plan for cracking the WiFi passcode much easier to understand; each iteration of the inner for-loop tries different numbers for the latest digit of the passcode.
## Event-Loop
JavaScript in the browser is single-threaded: at any time the JavaScript *runtime* can only execute thing at a time.

The key to understanding asynchronous JavaScript in the browser is this: the browser is more than just the JavaScript runtime.  At a systems level, concurrent programming has two strategies: use multiple *threads* or multiple *processes*.  The browser essentially uses the strategy of multiple *processes*, where the JavaScript runtime is just one of the "processes" that together make up the browser.  It is these other "processes" called **Web APIs** that can do stuff *concurrently* (in the background) while the JavaScript runtime is running.

Common Web APIs are:
- AJAX: handles fetch requests.
- Web Workers: these are essentially "forked" JavaScript runtimes that handle functions like `setTimeout()`.  They are of special consideration in the next section.

The JavaScript runtime model is called the **Event Loop**: it describes when asynchronous operations are placed back into the single thread of the runtime.  The event loop uses three data structures:
- The **Heap** is where JavaScript's objects and data are stored.
- The **Call Stack** is a stack that holds function calls and their closures (essentially pointers to arguments and local variables in the heap) in what are called *frames*; so a stack that holds frames.
- The **Message Queue** is a queue that holds callback functions (from asynchronous functions) and their closures in what are called *messages*.

So the steps of an Event Loop in executing a JavaScript program would be:
1. Place a function call onto the Call Stack, and run through the calls (as a stack, to be clear).
	-  If this function call leads to further function calls, these get placed on the Call Stack too.
	-  If the function call makes a call to an asynchronous function, this gets calls the appropriate Web API.
		- When the Web API finishes, it places a message in the Message Queue.
2. When the Call Stack is empty, check if the Message Queue has any messages.
	1. Run

In this way, the Event Queue acts like a message queue in Interprocess Communication.  The Event Queue serves as a *standby queue* for whenver there is downtime in the Call Stack.

With this bigger picture, we can fill some additional, smaller details:
- The Message Queue can also 
- We can now see that the milliseconds argument to `setTimeout()` is the *minimum* time that passes before the callback function executes.  The given milliseconds is how long the Web API will wait before passing the callback into the queue; and then this callback will need to wait for however long for the callbacks ahead in the queue to finish before getting put on the call stack.
- In standalone JavaScript engines (outside of the browser), the Web APIs are replaced by libraries.  In the case of Node.js, these are C++ libraries, like Libuv for handling asynchronous requests to databases.
## Web Workers
Among the Web APIs that can be invoked to provide asychronous capabilities to JavaScript, 

## WebAssembly
