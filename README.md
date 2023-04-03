<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>

<div align="center">
  
<h3 align="center">Kingfish</h3>

<div align="center">

  <a href="https://github.com/colding10/Kingfish">
    <img src="images/logo.jpeg" alt="Logo" width="160" height="160">
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

[![Product Name Screen Shot][product-screenshot]](https://example.com)

**Disclamer: this is a work in progress. The bot sucks. There is definitely an implementation bug i have not yet found. This may or may not get worked on in the future. Community contributions are surely needed for this to go anywhere.**

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* [![C++][C++]][C++-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites

This installation assumes MacOS X 10.14. Support for other operating systems or versions is not tested.

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

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->
## Roadmap

* [ ] Convert program (GUI) into library (while still maintaining interface)
* [ ] Implement engine interfaces (UCI) and advanced improvements
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

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Colin Ding - colinding10@gmail.com

Project Link: [https://github.com/colding10/Kingfish](https://github.com/colding10/Kingfish)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* []()

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/colding10/Kingfish.svg?style=for-the-badge
[contributors-url]: https://github.com/colding10/Kingfish/graphs/contributors

[forks-shield]: https://img.shields.io/github/forks/colding10/Kingfish.svg?style=for-the-badge
[forks-url]: https://github.com/colding10/Kingfish/network/members

[stars-shield]: https://img.shields.io/github/stars/colding10/Kingfish.svg?style=for-the-badge
[stars-url]: https://github.com/colding10/Kingfish/stargazers

[issues-shield]: https://img.shields.io/github/issues/colding10/Kingfish.svg?style=for-the-badge
[issues-url]: https://github.com/colding10/Kingfish/issues

[license-shield]: https://img.shields.io/github/license/colding10/Kingfish.svg?style=for-the-badge
[license-url]: https://github.com/colding10/Kingfish/blob/master/LICENSE.txt

[product-screenshot]: images/screenshot.png

<!-- Built With -->
[C++]: https://img.shields.io/badge/C++-20232A?style=for-the-badge&logo=nextdotjs&logoColor=white
[C++-url]: https://isocpp.org/