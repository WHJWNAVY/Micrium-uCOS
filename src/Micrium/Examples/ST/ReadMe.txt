CRYPTO EXAMPLE FOR THE ST STM32F746G-DISCO, REVISION 1

This example project demonstrates how to initialize uC/TCPIP with uC/HTTP to connect to a secure HTTP server, request a web resource, and print it to IAR's Terminal Output.
It can also host a simple web page that displays some of the Micrium product versions running on the board as well as its IP information, current UTC time, and uptime.


MICRIUM PRODUCT VERSIONS
- uC/OS-III v3.07.03
- uC/CPU    v1.31.04
- uC/LIB    v1.38.02
- uC/Common v1.01.01
- uC/TCPIP  v3.04.06
- uC/HTTP   v3.00.06
- uC/DHCPc  v2.10.01
- uC/DNSc   v2.00.04
- uC/Clk    v3.09.05
- uC/SNTPc  v2.00.01
- emSSL     v2.54.0a

IDE/COMPILER VERSIONS
- IAR/EWARM v8.20.1

HARDWARE SETUP
- Connect a USB Micro-B cable to the "USB ST-LINK (CN14)" connector to provide power.
- Connect a Ethernet from your local network into the board's Ethernet port

WORKSPACE LOCATIONS
- ST/STM32F746G_Disco/Crypto/OS3/IAR/Crypto.eww

BUILD CONFIGURATIONS
- Several build configurations are available with this project:
    source      A full source build that generates an executable
    libgen      A full source build that generates a library
    library     A partial source build that links with the library created by "libgen"
- The "library" configuration can be run by users who do not have access to the source code for Micrium products.
- Users with access to the product source code can copy it into Micrium/Software/<product> and use the "source" and "libgen" configurations.
- Select the appropriate configuration that suits your needs by clicking on Project->Edit Configurations...

USAGE INSTRUCTIONS
- Server example:
  - Define APP_CFG_HTTP_S_EN as DEF_ENABLED in app_cfg.h. (DEF_DISABLED by default)
  - Compile and load the project by clicking on "Download and Debug" or by pressing Ctrl+D.
  - Open the Terminal output IAR by going to the "View->Terminal I/O" in the drop-down menu.
  - Run the program and allow for both the TCPIP and HTTPs stacks to initialize.
  - Open a browser window and enter "https://" followed by the IP address given to the board by the DHCP server in your LAN. It should be shown in the Terminal I/O.
  - The browser displays a self-signed SSL certificate warning shortly thereafter, which is normal for this example. The server's certificate cannot be verified with
    a certificate authority because it is self-signed.
  - If using Chrome, click on "Advanced" then click on "Proceed to [IP address] (unsafe)".
    For Firefox, click on "Advanced" and then "Add Exception..."
    For Edge, click on "Continue to this webpage (not recommended)"
  - Once the index.html sample page is served, it'll display basic information about the board (MCU Name, IP Address, Software versions, as well as the uptime and UTC time). The
    name of the demo "Secure Webserver Demo" is also displayed as well as a link on the upper left corner titled "Micrium Network Stacks" that takes the user to our online 
    product information page for our networking suite.

- Client example:
  - Define APP_CFG_HTTP_C_EN as DEF_ENABLED in app_cfg.h.
  - Compile and load the project by clicking on "Download and Debug" or by pressing Ctrl+D.
  - Open the Terminal output IAR by going to the "View->Terminal I/O" in the drop-down menu.
  - Run the program and allow for both the TCPIP and HTTPc stacks to initialize.
  - The application task AppTaskStart() will attempt to connect to the server https://mqtt.micrium.com/ssldemo:8080 and request the contents of the web page and print it to the 
    Terminal I/O window. If successful, the text "Micrium SSL Demo" should appear on the Terminal every 5 seconds.

- Please note:
    1)  For these examples, emSSL was configured to only use the RSA key exchange algorithm in net_secure_emssl_cfg.h file.
    2)  emSSL only supports DER-formatted certificates which can be converted to a C structure by using SEGGER's PrintCert.exe command-line utility.
    3)  The total number of SSL sessions are determined by the sum of NET_SECURE_CFG_MAX_NBR_SOCK_SERVER and NET_SECURE_CFG_MAX_NBR_SOCK_CLIENT in net_cfg.h.
    4)  If at any point the user would like to disable emSSL, it can be done by redefining NET_SECURE_CFG_EN as DEF_DISABLED in net_cfg.h. In the client example the connection 
        attempt to the secure server will not be performed and the server example will serve the plaintext version of the index.html with the demo name changed to "Webserver Demo"
        instead.
	
Please feel free to post questions or comments related to this example project at Micrium's forum page:

https://www.micrium.com/forums/topic/st-stm32f746g-disco-crypto/