<p align="center">
 <img src="https://raw.githubusercontent.com/Cylix/netflex/master/assets/images/netflex_logo.jpg"/>
</p>

# NetFlex [![Build Status](https://travis-ci.org/Cylix/netflex.svg?branch=master)](https://travis-ci.org/Cylix/netflex)
`NetFlex` is a modern C++11 HTTP Server.

## Requirement
`NetFlex` has **no dependency**. Its only requirement is `C++11`.

**This library is still under development**

## Motivations and goals
Serving HTTP requests from a C++ software might seem unconventional, but some projects do require it (and I personally already encounter such need).

However, even though C++ is getting older with a more and more complete standard library and a huge active community, tools for this kind of tasks are very limited.
A few solutions exist hopefully, including `cpp-netlib` which is the main and maybe only usable C++ library providing HTTP server & client features.

However, `cpp-netlib` could be lot better. Its design is pretty old, the documentation is a real mess and the resulting library is not stable enough.

`NetFlex` aims to provide an alternative to `cpp-netlib` and other smaller C++ HTTP libraries with a modern C++ design.
One of its goal is to be as much flexible as possible in order for the users to integrate it easily in their existing program while providing the core features to fulfill HTTP support.

`NetFlex` will be compliant with the following HTTP RFC:

* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing](https://tools.ietf.org/html/rfc7230) (June 2014)
* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Semantics and Content](https://tools.ietf.org/html/rfc7231) (June 2014)
* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Conditional Requests](https://tools.ietf.org/html/rfc7232) (June 2014)
* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Range Requests](https://tools.ietf.org/html/rfc7233) (June 2014)
* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Caching](https://tools.ietf.org/html/rfc7234) (June 2014)
* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Authentication](https://tools.ietf.org/html/rfc7235) (June 2014)

That is, `NetFlex` aims to be `HTTP/1.1` RFC compliant.
Support will be first provided for `HTTP`, then `HTTPS` in a second time and lastly `HTTP/2.0`.

## Example

*No example available yet*

## Wiki
A [Wiki](https://github.com/Cylix/netflex/wiki) is available and provides full documentation for the library as well as [installation explanations](https://github.com/Cylix/netflex/wiki/Installation).

## License
`NetFlex` is under [MIT License](LICENSE).

## Contributing
Please refer to [CONTRIBUTING.md](CONTRIBUTING.md).

## Author
[Simon Ninon](http://simon-ninon.fr)
