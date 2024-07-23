Smart Asset Tracking System Using UHF RFID Reader

Developed-an-asset-tracking-system-using-UHF-RFID-READER
The project aims to design and implement a Smart Asset Tracking System utilizing advanced technologies like Radio Frequency Identification (RFID). This system seeks to revolutionize asset management practices by automating tracking processes, reducing manual intervention, enhancing real-time visibility of assets, and optimizing security measures. Ultimately, it aims to mitigate risks associated with asset mismanagement and unauthorized access, thereby improving operational efficiency and security within organizations.
The system includes an application with a login page for user authentication. Upon successful login, users are redirected to a dashboard offering features such as:

1.Add Asset: Allows users to add new assets to the system.
2.Asset Details: Provides real-time status and details of each asset.
3.Asset Permissions: Enables users to manage permissions associated with assets, facilitating controlled access for experiments or other authorized activities.

Developed an asset tracking system using UHF RFID reader: Implement one UHF reader in front of the lab and another at the main gate near the security guard. When an unauthorized asset passes through these checkpoints, a buzzer will sound and an email notification will be sent. Real-time location information of assets will be tracked to determine whether they are inside the lab or outside the campus.
The first step involves labeling all assets with RFID tags and adding their details, including tag numbers, to the server through a dedicated mobile application. The UHF RFID reader is connected to the STM32G431rb via RS232. When the RFID reader detects a tag, it verifies the tag number. If the tag number matches, it checks the count associated with that tag. An odd count indicates the asset is outside the lab, while an even count indicates it is inside.
Similar procedures are implemented at the main gate, with additional focus on preventing asset theft. If an unauthorized asset attempts to pass through without permission, the RFID reader will detect it, triggering the buzzer and sending an email alert.

UHF RFID Reader and Tag Working:

The working principle of a UHF RFID reader revolves around radio frequency identification
technology, enabling the wireless transfer of data between the reader and RFID tags. UHF RFID
readers emit radio waves via an antenna, creating an electromagnetic field in the reader’s vicinity.
When an RFID tag enters this field, it absorbs some of the energy and uses it to power its circuitry.
The tag then modulates the electromagnetic field by reflecting back a portion of the energy with its stored data encoded in the modulation. The UHF RFID reader detects this modulated signal,
demodulates it, and decodes the data contained within the RFID tag.
The reader’s antenna plays a crucial role in transmitting and receiving radio waves efficiently.
Depending on the specific application and environmental factors, UHF RFID readers can have
different antenna configurations to optimize read range, directionality, and interference rejection.

UHF RFID readers typically support various communication interfaces such as Ethernet, RS-
232, RS-485, USB, and Wi-Fi, allowing for seamless integration with different systems and devices.

They also support multiple RFID protocols like EPC Gen2, ISO 18000-6C, ensuring compatibility
with a wide range of RFID tags and systems.
In essence, UHF RFID readers enable the automatic identification and tracking of objects
or assets in diverse applications such as inventory management, supply chain tracking, access
control, and asset tracking. Their ability to read tags from several meters away, along with fast
data transfer rates, makes them suitable for applications where long-range and high-volume RFID
reading is required.

Circuit Components:

Hardware:
1. STM32 nucleoG431RB - 2
2. UHF RFID Reader - 2
3. RS-232
4. Buzzer
5. ESP8266 wifi module(WeMos D1 Mini(LOlin D1 R1))-2
6. Jumper Wires
7. Wifi Network

Software:
1. STM32 Cube IDE
2. Arduino IDE
3. SMTP Server for E-Mail Generation

Block Diaghram:

![image](https://github.com/user-attachments/assets/8df0111d-21ad-4b1b-ad6f-180b8be8c8be)

Circuit Design:

![Circuit Design at exit point](https://github.com/user-attachments/assets/f2a32413-f19f-483d-b2aa-37e3c361146c)

![Circuit Design at Lab1](https://github.com/user-attachments/assets/88baa9c2-8958-42ca-85c3-7ec6b64b4d0b)

Asset tracking App:

In my project, I extensively documented the development of my application using Visual studio.One of the key functionalities I implemented was the authentication system, comprising
both sign-in and sign-up features. Users could input their credentials securely, and upon successful authentication, they were directed to the asset view section. Here, users could not only
view existing assets but also add new ones. Additionally, I integrated a feature that requested permission to access the device’s current location, enhancing the application’s functionality. This
permission was crucial for users to accurately identify and manage assets based on their real-world locations. By seamlessly integrating these features into the application, I aimed to create a robust
and user-friendly experience for the end-users, which I believe is fundamental to the success of any application.

1.Sign Up and Sign In
![Sign Up and Sign In](https://github.com/user-attachments/assets/61af36d0-5dbe-4d14-b15b-a9ba6484d3b5)

2.After SignIn
![After SignIn](https://github.com/user-attachments/assets/39d6d7ff-2682-4a46-85b1-77cfbf946129)

3.Add Asset
![Add Asset](https://github.com/user-attachments/assets/2f615766-1967-4630-8f0a-33ec17c902c5)

4.Add permission
![Add permission](https://github.com/user-attachments/assets/5f9ba90d-266e-4074-925f-b7ec05221f66)

5.Asset_details
![Asset_details](https://github.com/user-attachments/assets/4faf8c71-22f9-4e06-bb8f-252bbf90760e)

Conclusion:

In response to the increasing demand for efficient asset tracking and security measures, our project introduces a Smart Asset Tracking System leveraging Radio Frequency Identification (RFID)
technology. This system aims to revolutionize asset management practices and enhance security measures within educational institutions and other organizations.
By developing a robust RFID-based tracking system, we address the limitations of traditional asset tracking methods, such as manual record-keeping and barcode scanning. The system offers
real-time visibility into the movement and status of assets, enabling stakeholders to track asset utilization efficiently and optimize inventory management processes.

If an unauthorized object passes through security gate, it will check its permissions. If the object lacks permission, a buzzer will sound and an email will be sent.
![Theft_Alert](https://github.com/user-attachments/assets/eb07dffe-e922-4cad-9dc3-9b9dd1324433)


