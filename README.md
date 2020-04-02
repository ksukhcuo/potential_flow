# Potential_Flow

This program calculates 2-dimensional potential flow around the quadrangular prism, and visualizes it. Source codes are written in c++.

It solve 2-dimensional Laplace's equation using Successive Over-Relaxation method (SOR), and take gradient of the potential to obtain the velocity field.

## Requirements

This requires following packages.

* g++
* gnuplot

I have confirmed that it works with versions 6.3.0 and 5.2, respectively. And it is required that both are added to PATH.

## Usage

Changing to "potential_flow" directory, and run

>`make`

## License

This software is released under the MIT License, see LICENSE.