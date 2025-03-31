# The Parker1-Transmitter / Transceiver
The design of a ten band HF transmitter / transceiver and winner of the G-QRP Club Memorial Trophy 2024.

Please see the attached submission to the G-QRP Club for a full description of the project.

The transmitter consists of two parts, the Nano VFO and the HF Multi-band PA, connected together to make the transmitter. 
Note that the HF PA board is designed to have the QRPLabs LPF Relay board mounted on it to provide five LPFs covering ten bands (160-10m.)

Since this submission, the transmitter is now a transceiver, with the addition of a QRPLabs RX module, 
Hi-Per-Mite CW filter and audio amp. This is not protrayed in the diagrams here, but the circuit diagrams of the TX and VFO should make connections clear.
The L-Match ATU and internal battery pack are also not depicted, and there are no diagrams for this, but you should be able to work this out yourself.
All that is shown here is the code, circuit diagrams and PCB files for the VFO and for the PA board, nothing else,
however it is a great place to start and then experiment further.

I AM SORRY BUT I AM TOO BUSY TO OFFER SUPPORT OR ANSWER QUESTIONS (I have already answered a lot!) Please do not attempt this project without being
aware of that and reasonably confident in your abilities. My advice would to be just build the VFO first. The provided transceiver code will still work with just the VFO, but be aware that the code only activates the Si5351 clocks for TX and TX when those modes are in operation. In other words, to use just the VFO you would need to modify the clock output code to acheive a permanent clock / VFO output if that is what is required.

You will need to modify the code yourself to select your starting frequency and your callsign / CQ message. I have tried to make the code clear with plenty of remarks.
You will also need to amend the code to choose the RX frequency settings (i.e. offset, etc.) These options are NOT menu driven and must be done within the code
before uploading to the VFO. Therefore a degree of construction and programming experience is absolutely required.

The project has attracted quite a bit of interest since recently winning the G-QRP Club 2024 Construction Competition (G3RJV Trophy) and the intent
of this repository is to assist those interested in building it or using it as a platform for their own projects. I am sorry I am unable to help further but
hope this is of use.

Good luck.
Daimon, G4USI.
