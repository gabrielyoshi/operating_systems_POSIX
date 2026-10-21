# File Management System

A small simulated filesystem written in C to learn operating system concepts.

## Current progress

- Simulated disk: 100 blocks (per instuctor requirements) of 64 bytes each 
- File metadata table in struct and free-space bitmap list of bool
- Metadata initialization and file listing
- A sample metadata entry for testing the listing function

## Planned features

- Create File: Create a new file and store its metadata.
- Delete File: Remove a file and release its allocated blocks.
- Write File: Write data into allocated disk blocks.
- Read File: Retrieve previously stored data.
- List Files: Display file names, sizes, and allocated blocks.
- Disk Allocation: Contiguous disk allocation (or linked allocation)
- Bitmap-based free-space management

## Written Formal Report
- Abstract, Introduction, Body, Conclusion
  - Description of a general file management system
    - Example of modern file management system
      - Research and comparison of other file systems
      - File system used in Mac, PCs, and flash drive
      - Process of copying one file system into another file system (information loss, 
  - Description of file management system implemented in submission
  - Conclusion: Learnings
