GPU Accelerated Viewshed Calculations
=====================================

A lot of telcos / rural network providers need to place radio masts to serve their customers. The ideal placement
of these masks can be hard, needing to balance a number of hard, technical requirements (power, line of sight, etc)
with soft, social requirements (land rights, planning permission, etc).

Tools currently exist for calculating mast lines of sights ([viewsheds](https://en.wikipedia.org/wiki/Viewshed)), but
they tend to be proprietary, slow, or not automatable. My honours project as part of the completion of my MEng Software
Engineering programme at Napier University intends to:

1. Investigate the validity and effectiveness of GPU acceleration of viewshed calculations
2. Develop a core library that allows developers to embed such acceleration into tools
3. Develop example tools using this library.
