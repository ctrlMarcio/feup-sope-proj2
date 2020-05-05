# feup-sope-proj2

[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/ctrlmarcio/feup-sope-proj2/C%20CI%20Build?logo=github)](https://github.com/ctrlMarcio/feup-sope-proj2/actions?query=workflow%3A%22C+CI+Build%22)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/ctrlmarcio/feup-sope-proj2/C%20CI%20Documentation?label=documentation&logo=github)](https://github.com/ctrlMarcio/feup-sope-proj2/actions?query=workflow%3A%22C+CI+Documentation%22)
[![Codacy grade](https://img.shields.io/codacy/grade/f8bebb649ff54000894035a9db01d11d?logo=codacy)](https://app.codacy.com/manual/ctrlMarcio/feup-sope-proj2)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/ctrlmarcio/feup-sope-proj2?logo=github)](https://github.com/ctrlMarcio/feup-sope-proj2/actions?query=workflow%3A%22C+CI+Release%22)
[![GitHub license](https://img.shields.io/github/license/ctrlMarcio/feup-sope-proj2?color=blue)](https://github.com/ctrlMarcio/feup-sope-proj2/blob/master/LICENSE)

This repository comprises the second project of the Operating Systems course unit project. The goal was to create a solution for a virtual access to a bathroom via FIFOs and threads.

## Project description

It is intended to obtain a client-server application capable of handling conflict situations in access to shared memory regions.

The shared memory region is a bathroom with several unisex seats,
controlled by a Q process, which receives requests from users. Access requests are sent through a multi-threaded process U (client), indicating the time that the users wishes be in the bathroom.

## Files

The source files are located inside the **src** directory. The src directory has three subdirectories. The **server** directory is related to the server program, the **client** directory is related to the client program and the **util** directory contains useful files for both programs.

## Documentation

The documentation is generated automatically with **doxygen** and can be accessed [here](https://ctrlmarcio.github.io/feup-sope-proj2/).

## Compiling

Run make in the root directory of the repository and the binary client and server files are placed inside a **bin** directory in the repository root.

## Running

### Client

To run the client program use:

```bash
./bin/U1 -t <number of seconds> <FIFO name>
```

-   **number of seconds**, the number of seconds that the program must send requests;
-   **FIFO name**, the name of the server's public FIFO.

### Server

To run the server program use:

```bash
./bin/Q1 -t <number of seconds> <FIFO name>
```

-   **number of seconds**, the number of seconds that the program must receive requests;
-   **FIFO name**, the name of the public FIFO.

### Note

The FIFOs are stored in the **/tmp/** directory of the OS.

### Exit codes

| Exit Code |         Name        | Description                                            |
| :-------: | :-----------------: | ------------------------------------------------------ |
|     1     |   Arguments Error   | Defines an error with the arguments.                   |
|     2     |   Connection Error  | Defines an error connecting the client and the server. |
|     3     | FIFO Creation Error | Defines an error while creating or opening a FIFO.     |
|     4     |   Alarm Init Error  | Defines an error when creating an alarm.               |
|     5     |  Thread Init Error  | Defines an error when initializing a thread.           |

## Features

Each feature is classified as bad, ok or great.

|                Feature               |                                                     Status                                                    |
| :----------------------------------: | :-----------------------------------------------------------------------------------------------------------: |
|            SIGINT Handling           |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|     Argument Reading and Handling    |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|     Multi-thread Request Handling    |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|      Regular Request Processing      |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
| Irregular Request Processing (error) |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|         Operation Registering        |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|          Deadlock Situations         |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|         Synced Memory Access         |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|     Resource Removal and Release     |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|          Pipe Communication          |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|          SIGPIPE Handling\*          |   ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)   |
|                Hotel?                | ![https://img.shields.io/badge/trivago-%20-brightgreen](https://img.shields.io/badge/trivago-%20-brightgreen) |

### Notes

**\*SIGPIPE Handling** - whenever a pipe is force closed (_e.g._ SIGINT, etc) any process that had the pipe opened receives a SIGPIPE. This was handled in order to continue the program after a SIGPIPE in any of the program's thread.

## Tests

To run the tests run the shell script **test_runner.sh** in the **test** directory. The test verifies if the number of 2LATE logs are equal to the number of CLOSD logs. Log and err files are created in the test directory.

```bash
./test/test_runner.sh <server seconds> <client seconds> <FIFO name>
```

The output is **OK** if the test is successful, **FAILED** otherwise.

## Authors

-   Márcio Duarte | [GitHub](https://github.com/ctrlMarcio) \| [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201909936)
-   Luís Tavares | [GitHub](https://github.com/luist18) \| [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201809679)

### Disclaimer

This repository contains the **C** code solution for the problem. Note that it might contain errors and should not be used as a solution.

### License

[MIT](https://opensource.org/licenses/MIT)
