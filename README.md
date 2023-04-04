<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>

<div align="center">
  
<h3 align="center">Kingfish</h3>

<div align="center">

  <a href="https://github.com/colding10/Kingfish">
    <img src="https://raw.githubusercontent.com/colding10/Kingfish/adfcef6b83760c19fe876664bf6e46cd3c141152/images/logo.jpeg" alt="Logo" width="160" height="160">
  </a>

  [![Contributors][contributors-shield]][contributors-url]
  [![Forks][forks-shield]][forks-url]
  [![Stargazers][stars-shield]][stars-url]
  [![Issues][issues-shield]][issues-url]
  [![MIT License][license-shield]][license-url]

</div>

  <p align="center">
    A C++ implementation of a playable chess game. This turned into my own chess engine that I am continuously improving.
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
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

`Kingfish` is a UCI chess engine built in C++. It was written from scratch, however it was inspired by Stockfish and Sunfish, two very popular chess engines. My dream is that this engine can eventually play at levels above 1800 ELO, and will eventually create a [LiChess](lichess.org) account for it to get ranked.

The engine supports a few but not all of the common UCI commands. It is tailored to work with Cute Chess, an application for playing UCI chess engines.

As of right now, it sometimes makes illegal moves and is still in beta development.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* [![C++][C++]][C++-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up Kingfish locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites

This installation assumes macOS. Support for other operating systems is not tested.
Kingfish has been tested on macOS 10.14 and macOS 12.

* [Homebrew](https://brew.sh) (if not installed)

  ```bash
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  ```

* SDL2

  ```bash
  brew install SDL2
  ```

* SDL2_image
  
  ```bash
  brew install SDL2_image
  ```

* SDL2_ttf

  ```bash
  brew install SDL2_ttf
  ```

### Installation

1. Clone the repo

   ```sh
   git clone https://github.com/colding10/Kingfish.git
   ```

2. Compile

   ```sh
   make
   ```

The binary should be created into the folder bin.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

See `engine-interface.txt` for a description of the UCI protocol.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->
## Roadmap

* [x] Convert program (GUI) into library (while still maintaining interface)
* [x] Implement basic engine interfaces (UCI)
* [ ] _Play test games and debug_
* [ ] Implement all engine interfaces (UCI)
* [ ] Add advanced search features to improve speed
* [ ] Play engine against other programs to rank ELO

See the [open issues](https://github.com/colding10/Kingfish/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Kingfish is free and distributed under the
[**GNU General Public License version 3**][license-url] (GPL v3). Essentially,
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

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Colin Ding - colinding10@gmail.com

Project Link: [https://github.com/colding10/Kingfish](https://github.com/colding10/Kingfish)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Stockfish](https://github.com/official-stockfish/Stockfish)
* [Sunfish](https://github.com/thomasahle/sunfish)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/colding10/Kingfish.svg?style=for-the-badge
[contributors-url]:    https://github.com/colding10/Kingfish/graphs/contributors

[forks-shield]:        https://img.shields.io/github/forks/colding10/Kingfish.svg?style=for-the-badge
[forks-url]:           https://github.com/colding10/Kingfish/network/members

[stars-shield]:        https://img.shields.io/github/stars/colding10/Kingfish.svg?style=for-the-badge
[stars-url]:           https://github.com/colding10/Kingfish/stargazers

[issues-shield]:       https://img.shields.io/github/issues/colding10/Kingfish.svg?style=for-the-badge
[issues-url]:          https://github.com/colding10/Kingfish/issues

[license-shield]:      https://img.shields.io/github/license/colding10/Kingfish.svg?style=for-the-badge
[license-url]:         https://github.com/colding10/Kingfish/blob/master/LICENSE.md

<!-- Built With -->
[C++]: https://img.shields.io/badge/C++-20232A?style=for-the-badge&logo=nextdotjs&logoColor=white
[C++-url]: https://isocpp.org/
