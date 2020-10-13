# <img src="https://raw.githubusercontent.com/abansal755/Image-Editor/readme/icon.png" height="25"/> Image-Editor

[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/abansal755/Image-Editor?include_prereleases&label=latest%20release)](https://github.com/abansal755/Image-Editor/releases)
![GitHub (Pre-)Release Date](https://img.shields.io/github/release-date-pre/abansal755/Image-Editor)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/abansal755/Image-Editor)](https://github.com/abansal755/Image-Editor/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors/abansal755/Image-Editor?color=brightgreen)](https://github.com/abansal755/Image-Editor/graphs/contributors)
[![issues](https://img.shields.io/github/issues/abansal755/Image-Editor)](https://github.com/abansal755/Image-Editor/issues)

# Introduction
>This is a node-based compositing image editor which represents an entire composite as a directed unweighted graph, linking media objects and effects in a procedural map, intuitively laying out the progression from source input to final output. This type of compositing interface allows great flexibility, including the ability to modify parameters of an earlier image processing step while viewing the final composite.

# Tools Used
- **Qt Creator**
>Qt is a free and open-source widget toolkit for creating graphical user interfaces as well as cross-platform applications that run on various software and hardware platforms such as Linux, Windows, macOS, Android or embedded systems with little or no change in the underlying codebase while still being a native application with native capabilities and speed.

# Download
>You can download the latest installable version of [ImageEditor](https://github.com/abansal755/Image-Editor/releases) for Windows, macOS and Linux.

# Usage
>* Once you have downloaded a version extract the zipped file and run the Image-Editor.exe file
>* Now right click on the canvas to select the type of node 
>* Once the nodes are created right click on each node's connector (a small white box) to connect the input and output of various nodes sequentially as per the tooltip.
>     > Tooltip : Right-click for creating edges, middle button for deleting edges
>* Right-click on each node for properties of each node or deleting the node
# Nodes 
>* Read node - Reads the image file from your system 
>* View node - A new QGraphicsview window pops for  for displaying the contents of a QGraphicsScene.
>* Write node - To save the modified image with desired compression.

#### All other nodes are image transfrormation nodes whose degree can be selected in their respective properties. 

>* Grade node - Used to change the visual tone of an image. In simpler terms, it is used to manipulate the colors of a photograph.
>* Gamma node - Increasing Gamma makes dark areas darker and light areas lighter.
>* Saturate node - Alter the intensity and purity of a color as displayed in an image. The higher the saturation of a color, the more vivid and intense it is. The lower a color's saturation, the closer it is to pure gray on the grayscale.
>* Contrast node -  To alter the difference in brightness between objects or regions. High contrast photos pop out, show textures in the subject and give a feeling of edginess, high energy and strength.
>* Blur node - Blurs the image by  creating a spectrum of colours at the edges of objects.It simply reduces the edge content and makes the transition form one color to the other very smooth.
>* Crop node - Crops the image by removing unwanted outer areas.
>* Scale node - Tells the image resolution in pixels and allows user to resize the image's resolutions.
>* Rotate node - Rotates the image with desired angle(both clockwise and anti-clockwise).
>* Mirror node - Mirrors the image with horizontal as well as vertical axis
>* Shuffle node - Shuffle node rearranges different color channels of the image into other color channels. 

# References
>* [Qt For Beginners](https://wiki.qt.io/Qt_for_Beginners)
>* [Complete Documentation](https://doc.qt.io/)
>* [Tutotiral](https://www.udemy.com/course/qt-c-gui-tutorial-for-complete-beginners/)

# Contributing
>We welcome pull requests from open source community and Qt developers alike! [Please follow these steps to contribute](https://github.com/Ojhaakshat/Image-Editor/blob/readme/contributing.md)

