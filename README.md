# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## Using SLCD controller on MCXC444 MCU
This application note provides a comprehensive guide on using the Segment Liquid Crystal Display (SLCD.) technology, specifically focusing on the NXP MCXC444 Microcontroller Unit (MCU) which integrates an SLCD controller module. It cover the usage of the S401M16KR SLCD device, a four-digit 0.17" seven-segment custom LCD panel, including hardware connections, basic usage, and operation in low power modes


![](image/frdm_mcxc444_slcd.jpg)




#### Boards: FRDM-MCXC444
#### Categories: HMI
#### Peripherals: DISPLAY
#### Toolchains: MCUXpresso IDE

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [FAQs](#step5) 
6. [Support](#step6)
7. [Release Notes](#step7)

## 1. Software<a name="step1"></a>
- Download and install [MCUXpresso IDE V11.9.0 or later](https://nxp.flexnetoperations.com/control/frse/download?agree=Accept&element=3595113).
- MCUXpresso for Visual Studio Code: This example supports MCUXpresso for Visual Studio Code, for more information about how to use Visual Studio Code please refer [here](https://www.nxp.com/design/training/getting-started-with-mcuxpresso-for-visual-studio-code:TIP-GETTING-STARTED-WITH-MCUXPRESSO-FOR-VS-CODE).

## 2. Hardware<a name="step2"></a>
* FRDM-MCXC444

  ![](image/frdm_mcxc444.jpg)
* Personal Computer
* USB type-C cable

## 3. Setup<a name="step3"></a>


### Step 1 (Import Project)
1. Open MCUXpresso IDE, in the Quick Start Panel, choose **Import from Application Code Hub**    

	[<img src="image/import_project_1.png" width="300"/>](image/import_project_1.png)

2. Enter the demo name in the search bar.   

	[<img src="image/import_project_2.png" width="300"/>](image/import_project_2.png)

3. Click **Copy GitHub link**, MCUXpresso IDE will automatically retrieve project attributes, then click **Next>**.   

	[<img src="image/import_project_3.png" width="300"/>](image/import_project_3.png)

4. Select **main** branch and then click **Next>**, Select the MCUXpresso project, click **Finish** button to complete import.   

	[<img src="image/import_project_4.png" width="300"/>](image/import_project_4.png)

   

### Step 2 (Hardware Setup)

Connect FRDM-MCXC444 board with MIKROE HEART RATE 4 CLICK module via MIKEOE bus interface. Compile the project and use a TypeC-USB cable to connect to the FRDM board via MCU-LINK connector. Use MCU-LINK to download program into MCU. 

![](image/frdm_mcxc444_plug_in.jpg)

### Step 3 (Running The Demo)

Compile the project and download program into MCU. Press SW1 on FRDM board to reset the board. open serial terminal with 115200,N8N1.

## 4. Results<a name="step4"></a>
There are two projects for this AN: 

* frdmmcxc444_slcd_basic: basic SLCD show and usage
* frdmmcxc444_low_power: SLCD keep display while MCU enter low power mode.

Both demo need user input on serial terminal. For details of those two demos, please see the AN content.

## 5. FAQs<a name="step5"></a>
No FAQ currently

## 6. Support<a name="step6"></a>
For additional information and support, visit NXP's official website:

#### Project Metadata
<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXC444-blue)](https://github.com/search?q=org%3Anxp-appcodehub+FRDM-MCXC444+in%3Areadme&type=Repositories)

<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-HMI-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+hmi+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-DISPLAY-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+display+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected funcionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/@NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/Twitter-Follow%20us%20on%20Twitter-white.svg)](https://twitter.com/NXP)

## 7. Release Notes<a name="step7"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
| 1.0     | Initial release on Application Code Hub        | May 14<sup>th</sup> 2024 |

