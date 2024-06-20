# Browsers
## Background
The **internet** is a network of computers connected to send information between each other.  The sent information through rules called **network protocols**, which standardize how computers send/receive and interpret information.

The **World Wide Web** is a set of these protocols (along with some other specifications) for sending and receiving web pages.  A **web page** is an HTML document and accompanying resources that presents some human-readable information.  A collection of web pages that operate together is called a **website**.  Computers that host and send-out web pages upon request are called **web servers**.

A computer uses an application called a **web browser**, or simply browser, to request and view web pages.  From a perspective, a browser is an application that manages World Wide Web connections and resources.

## Browser Components
The major componenets of a browser are:

- **User interface**
- **Browser Engine**
- **Rendering Engine**
- **JavaScript Engine**

The **User Interface** refers to the typical GUI or TUI tools like an address bar or search engine that can be used to request web pages.  The **Browser Engine** serves as the intermediary between the User Interface and the Rendering Engine; in other words, requests from the User Interface are made into actual commands to the Rendering Engine.  The browser engine also performs other tasks, like interfacing with the host filesystem.

The **Rendering Engine** is responsible for displaying a web page in a human-viewable format.  The **JavaScript Engine** executes any scripts that come as part of a web page; the JavaScript Engine (also called the JavaScript interpreter or implementation library) is typically part of the Rendering Engine.

The Rendering Engine itself is made of many components:
- A parser, that reads HTML into a Document-Object Model (DOM).
- A sylizer, that parses CSS.
- As mentioned, the *JavaScript Engine* is part of/managed by the Rendering Engine.
- A *Networking Component* that fetches web pages and resources.

All major Rendering Engines are updated to also execute WebAssembly programs alongside JavaScript ..

As an example, the Firefox browser:

- The browser engine is *Quantum*.
- The rendering engine is *Gecko*.
- The JavaScript/WebAssembly Engine is *SpiderMonkey*.
- The Networking Component of Gecko is *Necko*.

In comparison, the Chromium browser:

- The browser engine is *Chromium*.
- The rendering engine is *Blink*.
- The JavaScript/WebAssembly implementation library is *V8*.

In reality, the distinction between these components are not always clear, nor are they entirely separate.  For example, *Blink* is referred to both as the browser engine and rendering engine.

There are three major rendering engines in use today:
1. Blink - used in Chrome and Chromium-based browsers like Edge, Brave, and Opera.
2. WebKit - used by Apple's Safari and ancestor of Blink.
3. Gecko - used in Mozilla's Firefox.
## Browsers as Processes
Modern browsers can view several web pages at once in separate **tabs**.  As such, modern browsers are *multi-process* and *multi-threaded* applications.

The Chromium browser has an easier to understand structure:
- The Browser Engine (UI) is a single process
- Every tab (web-page) is a single multi-threaded process, called a **content process**, each with its own rendering engine (Blink):
	- A main thread that runs the Blink renderer's main thread
	- A thread for the network component (handles IPCs and network requests)
	- Some special purpose threads
	- A pool of general purpose threads (Web Workers)

The Firefox browser organizes itself a little differently:
- The Browser Engine (UI) is a single process
- Four processes called **web content processes** that run a separate renderer engine (Gecko):
	- Every web page is a thread (**isolated web content**) of one of these web content processes.
	- Additional threads are branched from these web content processes.
- Additional **helper processes** e.g. for the GPU, networking

As such, Chromium based browsers offer maximum performance at the cost of memory while Firefox tries to balance performance and memory usage.

## JavaScript and WebAssembly
To provide 

## Links

[Firefox Source Documentation](https://firefox-source-docs.mozilla.org/index.html)

