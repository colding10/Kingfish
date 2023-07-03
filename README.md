<div align="center">

<h3 align="center">Kingfish</h3>

<div align="center">
  <a href="https://github.com/colding10/Kingfish">
      <img src="https://raw.githubusercontent.com/colding10/Kingfish/adfcef6b83760c19fe876664bf6e46cd3c141152/images/logo.jpeg" alt="Logo" width="400" height="400">
  </a>
  
  [![Build][build-badge]][build-link]
  [![Contributors][contributors-shield]][contributors-url]
  [![Forks][forks-shield]][forks-url]
  [![Stargazers][stars-shield]][stars-url]
  <br/>
  [![Release][release-shield]][release-link]
  [![Commits][commits-shield]][commits-link]
  <br/>
  [![Issues][issues-shield]][issues-url]
  [![MIT License][license-shield]][license-url]
</div>

  <p align="center">
    A C++ UCI-speaking chess engine. Kingfish is rated at ~2000 ELO on <a href="https://lichess.org/@/KingfishBot">lichess.org</a>.
    <br />
    <a href="https://github.com/colding10/Kingfish"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/colding10/Kingfish">View Demo</a>
    ·
    <a href="https://github.com/colding10/Kingfish/issues">Report Bug</a>
    ·
    <a href="https://github.com/colding10/Kingfish/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

`Kingfish` is a UCI chess engine built in C++. It was written from scratch, however it was inspired by Stockfish and Sunfish, two very popular chess engines. The engine was roughly estimated via the [chess.com](http://chess.com) Stockfish ELO estimator to about 2000 ELO. Additionally, on [lichess.org](http://lichess.org), [it's rating](https://lichess.org/@/KingfishBot) is about 2000 ELO.

> __I created a lichess account! Play Kingfish here: https://lichess.org/@/KingfishBot.__

The engine supports __most but not all of the common UCI commands__. It is tailored to work with Cute Chess, an application for playing UCI chess engines.

__The engine plays a very capable and competitive game of chess. It blunders once in a while however, and there are a few kinks to be worked out.__

## Usage

Kingfish is a software program designed for playing chess on a computer. However, it lacks a graphical user interface (GUI) which is necessary to visually display a chessboard and to facilitate the input of moves. Third-party developers have created separate GUIs that can be found online. To use Kingfish with a particular GUI, consult the documentation provided with that GUI for instructions on integrating the two applications.

Kingfish speaks UCI, a protocol for chess engines. See `engine-interface.txt` for a description of the UCI protocol.

## Roadmap

* [X] Convert program (GUI) into CLI
* [X] Implement basic engine interfaces (UCI)
* [X] Play test games and debug
* [ ] _Improve engine_
  * [ ] Add bitboards
  * [ ] Bitwise storage of moves and pieces
  * [ ] Avoid copying positions
  * [ ] Threading
* [ ] Implement all engine interfaces (UCI)

See the [open issues](https://github.com/colding10/Kingfish/issues) for a full list of proposed features (and known issues).

<!-- CONTRIBUTING -->

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are __greatly appreciated__.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<!-- LICENSE -->

## License

Kingfish is free and distributed under the
[GNU General Public License version 3][license-url] (GPL v3). Essentially,
this means you are free to do almost exactly what you want with the program,
including distributing it among your friends, making it available for download
from your website, selling it (either by itself or as part of some bigger
software package), or using it as the starting point for a software project of
your own.

The only real limitation is that whenever you distribute Kingfish in some way,
you MUST always include the license and the full source code (or a pointer to
where the source code can be found) to generate the exact binary you are
distributing. If you make any changes to the source code, these changes must
also be made available under GPL v3.

Project Link: [https://github.com/colding10/Kingfish](https://github.com/colding10/Kingfish)

<!-- LINKS -->
[contributors-url]: https://github.com/colding10/Kingfish/graphs/contributors
[forks-url]: https://github.com/colding10/Kingfish/network/members
[stars-url]: https://github.com/colding10/Kingfish/stargazers
[issues-url]: https://github.com/colding10/Kingfish/issues
[license-url]: https://github.com/colding10/Kingfish/blob/master/LICENSE

[contributors-shield]: https://img.shields.io/github/contributors/colding10/Kingfish.svg?style=for-the-badge&color=success
[forks-shield]: https://img.shields.io/github/forks/colding10/Kingfish.svg?style=for-the-badge&color=success
[stars-shield]: https://img.shields.io/github/stars/colding10/Kingfish.svg?style=for-the-badge&color=success
[issues-shield]: https://img.shields.io/github/issues/colding10/Kingfish.svg?style=for-the-badge&color=success
[license-shield]: https://img.shields.io/github/license/colding10/Kingfish.svg?style=for-the-badge&color=success
[release-shield]:      https://img.shields.io/github/v/release/colding10/Kingfish?style=for-the-badge&label=official%20release
[commits-shield]:      https://img.shields.io/github/commits-since/colding10/Kingfish/latest?style=for-the-badge

[build-link]:         https://github.com/colding10/Kingfish/actions/workflows/cmake.yml
[build-badge]:        https://img.shields.io/github/actions/workflow/status/colding10/Kingfish/cmake.yml?branch=master&style=for-the-badge&label=kingfish&logo=github

[release-link]:       https://github.com/colding10/Kingfish/releases/latest
[commits-link]:       https://github.com/colding10/Kingfish/commits/master
