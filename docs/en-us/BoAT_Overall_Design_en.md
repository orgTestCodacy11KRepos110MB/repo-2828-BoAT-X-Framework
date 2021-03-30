# BoAT Overall Design


## Introduction

### Overview
This article describes the overall design of the BoAT system, including architecture design, subsystem functions, internal and external interfaces, key processes and key technology design descriptions, in order to guide the overall design of each subsystem and the writing of test strategies/specifications.  
The intended readers of this article are: BoAT SDK detailed designers.


### Abbreviated terms
|Term   |Explanation                  |
|:----- |:--------------------------- |
|ABI    |Application Binary Interface |
|BoAT   |Blockchain of AI Things      |
|RLP    |Recursive Length Prefix      |
|RNG    |Random Numeral Generator     |
|RPC    |Remote Procedure Call        |
|TEE    |Trusted Execution Environment|


## BoAT design goals
As a middleware of the IoT blockchain, BoAT should be easily and quickly transplanted to various IoT modules with minimal changes，The design of BoAT follows the following goals:
+ Hierarchical design  
+ Multiple blockchain protocols are supported  
+ Scalable design  
+ Secret key security design  
+ Provide corresponding C language interface contract automatic generation tools for different blockchains  


## The position of BoAT SDK in the entire blockchain network
As a middleware connecting IoT devices and blockchain, BoAT SDK's position in the entire interactive network is shown in Figure 3-1.
![BoAT position](/en-us/images/BoAT_Overall_Design_en-F3-1-Boat_Position.png)  
Figure 3-1 The position of BoAT in the blockchain interactive network



## BoAT implementation framework 
BoAT follows a hierarchical design, which is divided into interface layer, protocol layer, RPC layer, hardware dependency layer, general tools and utility programs. among them:   
+ The interface provided by the interface layer is for users to call to access the corresponding blockchain.  
+ The protocol layer provides services to the interface layer.  
+ The RPC layer provides services to the protocol layer.  
+ The hardware dependency layer provides encryption, signature, storage and other services for the wallet at the interface layer.  
+ The general tool layer is used to generate the C language interface of the smart contract and provide services such as data encoding and format conversion for the remaining layers.  
+ The utility program provides services such as data format conversion, message encoding and decoding to each layer.  
The overall framework of BoAT is shown in Figure 4-1.
![BoAT overall framework diagram](/en-us/images/BoAT_Overall_Design_en-F4-1-BoAT_Overall_Framework_Diagram.png)  
Figure 4-1 BoAT overall framework diagram

### Interface layer

#### Overview
The interface layer is located at the top layer of BoAT and provides users with access to each blockchain. The interface layer consists of two parts:  
+ Wallet interface
   * The wallet interface is the entrance to the BoAT SDK, and different blockchains have a common wallet entrance.
+ Transaction interface
   * Each blockchain provides a set of transaction interfaces with similar functions but different implementations.  

The wallet interface of the interface layer is supported by the hardware dependent layer. For the description of the hardware dependent layer, please refer to [Hardware dependency layer](#Hardware-dependency-layer)  
The transaction interface of the interface layer is supported by the protocol layer. For a description of the protocol layer, please refer to [Protocol layer](#Protocol-layer).  

#### Wallet interface

##### The data structure and function implementation list of the wallet
The wallet is a container. In addition to storing the information necessary to access the blockchain, the container also contains a key management system, which corresponds to the data structure and implementation method of the wallet.

The BoAT SDK runs on the application processor of the cellular module, and the resources of its operating environment are limited. For example, some cellular modules may not provide non-volatile memory access interfaces; on the other hand,From the user's point of view, some users may just want to create a temporary wallet for testing, but don't want to store it for a long time.In view of this, in the design of BoAT SDK, wallets are divided into two categories: persistent wallets and one-time wallets. Persistent wallets are stored in the non-volatile memory of the operating environment and will not be lost when power is off; one-time wallets are stored in the RAM of the operating environment and will be lost when power is off.  
  
In the data structure of the wallet,It should contain at least the following elements:  
  + Account private key
  + Blockchain network information
  + RPC interface information

In addition to the elements that must be included, the data structure of the wallet can optionally include:  
  + Account address  

The wallet should provide the following functions to achieve:
  + SDK initialization
  + SDK de-initialization
  + Wallet creation
  + Delete wallet
  + Wallet uninstall
  + Query wallet information according to the index
  + Private key settings
  + Private key generation
  + Private key verification

##### Brief description of wallet interface function realization

###### SDK initialization / de-initialization

+ SDK initialization:
SDK initialization should be done before using BoAT SDK. The contents of this interface include:
   1. Initialization of wallet list  
The wallet list is a data structure that contains a fixed number of wallets and wallet-related usage information. The wallet-related usage information includes wallet usage identification, wallet name, and blockchain to which the wallet belongs.The wallet list is a global resource. The initialization of the wallet list means that each member in the data structure is initialized once, such as initializing the use identifier as unused, and initializing the wallet as a null pointer.  
   2. Other global resource initialization  
If some third-party libraries used by the SDK need to be initialized before calling, they should be initialized here, such as the third-party library cURL used by the RPC layer.

+ SDK de-initialization:
After using BoAT SDK, you should do SDK de-initialization to release resources. The content of this interface includes:
   1. Perform wallet uninstall  
   2. Other global resource de-initialization  
If some third-party libraries used by the SDK need to be de-initialized before being called, they should be placed here for de-initialization, such as the third-party library cURL used by the RPC layer

###### Wallet operation

+ Wallet creation:
This interface is used to realize the creation of a new wallet or the reading of an already created wallet. The contents of this interface include:  
  1. Check if the wallet list is full  
  2. Decide whether to create a new wallet or read the created wallet according to the specific input parameters  
  3. Decide whether to create a one-time wallet or a persistent wallet according to the specific input  
  4. Perform wallet initialization of a certain blockchain according to specific input parameters, such as performing Ethereum wallet initialization, or performing PlatONE wallet initialization
+ Delete wallet:
This interface deletes a persistent wallet from non-volatile memory.  
Note: This interface does not uninstall the wallet from RAM. After the persistent wallet is deleted, it becomes a one-time wallet. The content implemented by this interface includes:
  1. Delete the specified wallet file from the non-volatile memory
+ Unload wallet:
This interface is used to unload a wallet from RAM.  
Note: This interface only unload the wallet from RAM and does not delete the wallet from non-volatile memory. The content implemented by this interface includes:
  1. Perform wallet de-initialization of a certain blockchain according to specific input parameters, such as performing Ethereum wallet de-initialization, or performing PlatONE wallet de-initialization
+ Query wallet information according to the index:
This interface is used to query the wallet. The content implemented by this interface includes:
  1. Return the corresponding wallet information address according to the specific input parameters (input parameters can be storage serial number index value, wallet address, etc.)  

###### Private key operation

+ Private key settings:
This interface is used to set the private key of the wallet. The private key should be kept carefully. The content implemented by this interface includes:
  1. Perform private key verification  
  2. Perform private key assignment  
  3. Generate public key from private key (optional)  
  4. Generate address by public key (optional)  

+ Private key generation:
This interface is used to generate a valid private key. The content implemented by this interface includes:  
  1. Generate a private key by generating a random number  
  2. Perform private key verification  
  3. If the verification fails, return to step 1 to regenerate the private key, and if the verification succeeds, return the generated private key  
+ Private key verification:
This interface is used to check whether the given private key is valid. The content implemented by this interface includes:  
  1. Check whether the private key is in the valid private key range  

#### Transaction interface

##### Transaction data structure and function realization list
A transaction is a signed message, transmitted through the blockchain network, and recorded on the blockchain. The functions of transaction interfaces provided by different blockchains are basically the same.  

For Ethereum/PlatONE:

The transaction should contain at least the following elements:
+ The data structure of the wallet
+ Transaction identification
+ Messages required for transactions  

Depending on specific needs, the transaction can also optionally include:
+ Synchronous/asynchronous identification  

The transaction should provide the following functions:
+ Wallet initialization
+ Wallet de-initialization
+ Account balance inquiry
+ Transaction initialization
+ Send transaction
+ Query transaction receipt
+ Stateless message call  
  
 

##### Brief description of Ethereum transaction interface function implementation
+ Wallet initialization:
  1. Set the blockchain contract address
  2. Set EIP-155 compatibility for transactions
  3. Set chain ID
+ Wallet de-initialization:
  The content implemented by this interface includes:
  1. Reset private key information
  2. Release of blockchain network information resources
  3. Call the web3 interface provided by the protocol layer: de-initialize the web3 interface
+ Account balance inquiry:
  The content implemented by this interface includes:
  1. Prepare message information required for account balance inquiry
  2. Call the web3 interface provided by the protocol layer: get the account balance
+ Transaction initialization:
  This interface is mainly implemented to initialize the fields in the transaction structure. In addition to the three fields of signature v, r, and s, the transaction structure of Ethereum also has six fields including nonce, gasPrice, gasLimit, recipient, value, and data.Please note that the setting of the Nonce field of the transaction is not implemented in this interface, but is set at the time the transaction is sent, because the user may create multiple transaction messages at the same time, and the order in which these messages are sent to the blockchain network is not sure. In addition, it should be noted that the Nonce and gasPrice fields should be obtained from the blockchain by calling the corresponding web3 interface of the protocol layer. This method requires access to the network and will generate a certain amount of network traffic. The content implemented by this interface includes:
  1. Set the GasPrice field of the transaction
  2. Set the GasLimit field of the transaction
  3. Set the Recipient field of the transaction
  4. Set the Value field of the transaction
+ Send transaction  
  The content implemented by this interface includes:
  1. Determine to send synchronous/asynchronous identification (if any)
  2. Identified as synchronous sending: call the raw transaction interface provided by the protocol layer: raw transaction synchronous sending
  3. Identified as asynchronous transmission: call the raw transaction interface provided by the protocol layer: raw transaction asynchronous transmission
+ Check transaction receipt:
  The content implemented by this interface includes:
  1. Prepare to query message information required for transaction receipt
  2. Call the web3 interface "get transaction receipt" provided by the protocol layer once every specified time until the timeout period is triggered
  3. Return query result
+ Send Ether  
  The content implemented by this interface includes:
  1. Set the Nonce field of the transaction
  2. Set the Value field of the transaction
  3. Set the Data field of the transaction
  4. Execute sending transaction
+ Stateless message call:
  The content implemented by this interface includes:  
  1. Prepare message information needed for stateless message call
  2. Call the web3 interface "blockchain stateless call" provided by the protocol layer  

##### Brief description of PlatONE transaction interface function implementation
Compared with Ethereum, the differences are listed below:
+ Transaction initialization  
In addition to the initialization steps described by Ethereum, PlatONE also:
  1. Set transaction type field  


It can be seen from the foregoing that the difference between PlatONE and Ethereum is very small. When designing the data structure and code implementation of PlatONE, the inheritance of the data structure and the reuse of code implementation should be considered, which not only reduces the amount of code, but also facilitates maintenance. For example, the composition of the transaction structure. The transaction structure of PlatONE has one more transaction type field than the transaction structure of Ethereum. Therefore, in the design of the data structure, a possible design idea is shown in Figure 4-2.  
![A possible design idea of data structure](/en-us/images/BoAT_Overall_Design_en-F4-2-Data_Structure.png)  
Figure 4-2 A possible design idea of data structure  
Figure 4-2 describes a possible data structure design idea of PlatONE. Please note that the transaction type field of PlatONE should be placed at the end of the data structure, and the integrity of the data structure of the multiplexed Ethereum should not be destroyed. If the integrity of the data structure of Ethereum is destroyed, the implementation methods related to the data structure in Ethereum will not be reused.


### Protocol layer
#### Overview
The protocol layer is located in the second layer of the BoAT SDK, which mainly implements the protocol part of each blockchain. For Ethereum series blockchains, their protocols are very similar, such as Ethereum and PlatONE.   
The protocol layer is supported by the RPC layer. Please refer to [Protocol layer](#Protocol-layer).  

#### Ethereum's protocol layer implementation
##### raw transaction interface
The raw transaction interface of the protocol layer provides services for "sending transactions" to the transaction interface of the interface layer.Raw transaction upwards should provide at least the following interfaces:
+ raw transaction sent asynchronously    

In addition, raw transaction can also choose to provide the following interfaces:
+ raw transaction sent synchronously

##### Brief description of raw transaction interface

+ raw transaction sent asynchronously  
This interface implements the data encoding of raw transaction, such as RLP encoding of each field, hash calculation, signature, etc., and calls the web3 interface within the protocol layer to send it to the blockchain, and returns directly without waiting for the transaction be confirmed. Data encoding is divided into two parts: incompatible with EIP-155 specification and compatible with EIP-155 specification. The content implemented by this interface includes:

   - If the data encoding is incompatible with EIP-155 specification:
  1. Perform RLP encoding on the six fields of nonce, gasPrice, gasLimit, recipient, value, and data of the transaction
  2. Calculate the keccak-256 hash value of the RPL encoding in the previous step
  3. Sign the hash value of the previous step, and get the three values of parity, r, and s
  4. Perform RLP encoding on the nine fields of transaction nonce, gasPrice, gasLimit, recipient, value, data, v, r, s, where v = parity + 27, parity, r, s have been given in the previous step
  5. Call the web3 interface "send rawtransaction" to send to the blockchain

   - If the data encoding is compatible with EIP-155 specification:
  1. Perform RLP encoding on the nine fields of transaction nonce, gasPrice, gasLimit, recipient, value, data, v, r, s, where v = chainID, r = 0, s = 0
  2. Same as step 2 of "Incompatible with EIP-155 specification"
  3. Same as step 3 of "Incompatible with EIP-155 specification"
  4. Perform RLP encoding on the nine fields of the transaction, including nonce, gasPrice, gasLimit, recipient, value, data, v, r, s, where v = Chain ID * 2 + parity + 35, and parity, r, s have been given in the previous step
  5. Same as step 5 of "Incompatible with EIP-155 specification"

+ raw transaction sent synchronously  
This interface executes "raw transaction asynchronous sending" and waits for the transaction to be confirmed after success or timeout. The content implemented by this interface includes:
  1. Execute raw transaction synchronously
  2. Execute query transaction receipt
  3. Wait for the transaction to be confirmed or return after timeout

##### web3 interface
In addition to the raw transaction interface, the protocol layer should also provide the following web3 interfaces to the upper layer:
+ web3 interface initialization
+ web3 interface de-initialization
+ Get the content stored in the specified location of the blockchain
+ Get the number of account transactions
+ Get blockchain gasPrice
+ Get account balance
+ Get transaction receipt
+ Blockchain stateless call
+ Send raw transaction  

Looking through the RPC-related documents of Ethereum, you can know that Ethereum provides about 64 RPC methods. In the BoAT SDK, we only implement the above. The reason is the same as the description in [Wallet interface](#Wallet-interface). The resources of the SDK operating environment are limited. The above-mentioned RPC methods are several methods commonly used in data on the blockchain. If customers need other RPC methods in the future, BoAT SDK will provide them in a customized way.

##### Brief description of web3 interface
+ web3 interface initialization  
  The content implemented by this interface includes:
  1. Web3 interface resource application,Such as the memory space application of RPC content,Application for the memory space of the json string used to request or respond from the blockchain,Request for memory space of the parsed result of the response json string
  2. Initialize web3 message ID
  3. Perform "RPC interface initialization" of the RPC layer
+ web3 interface de-initialization  
  The content implemented by this interface includes:
  1. The release of web3 interface resources, such as the release of memory space for RPC content,It is used to release the memory space of the json string that requests or responds from the blockchain,The memory space of the parsed result of the response json string is released.
  2. Perform "RPC interface de-initialization" of the RPC layer
+ Get the content stored in the specified location of the blockchain  
  The content implemented by this interface includes:
  1. web3 message ID increment
  2. Package the request message of  "Get the content stored in the specified location of the blockchain"
  3. Call the RPC method "web3_eth_getStorageAt" to send the request message to the blockchain
  4. Parse the received block chain response message and return the analysis result
+ Get the number of account transactions  
  The content implemented by this interface includes:
  1. web3 message ID increment
  2. Package the request message of  "Get account transactions"
  3. Call the RPC method "web3_eth_getTransactionCount" to send the request message to the blockchain
  4. Parse the received blockchain response message and return the analysis result
+ Get the gasPrice of the blockchain  
  The content implemented by this interface includes:
  1. web3 message ID increment
  2. Package the request message of  "Get the gasPrice of the blockchain"
  3. Call the RPC method " web3_eth_gasPrice " to send the request message to the blockchain
  4. Parse the received blockchain response message and return the analysis result
+ Get account balance  
  The content implemented by this interface includes:
  1. web3 message ID increment
  2. Package the request message of  "Get account balance"
  3. Call the RPC method " web3_eth_getBalance " to send the request message to the blockchain
  4. Parse the received blockchain response message and return the analysis result
+ Get transaction receipt  
  The content implemented by this interface includes:
  1. web3 message ID increment
  2. Package the request message of  "get transaction receipt"
  3. Call the RPC method " web3_eth_getTransactionReceiptStatus " to send the request message to the blockchain
  4. Parse the received blockchain response message and return the analysis result
+ Blockchain stateless call  
  The content implemented by this interface includes:
  1. web3 message ID increment
  2. Package the request message of "Blockchain stateless call"
  3. Call the RPC method " web3_eth_call " to send the request message to the blockchain
  4. Parse the received blockchain response message and return the analysis result
+ Send raw transaction  
  The content implemented by this interface includes:
  1. web3 message ID increment 
  2. Package the request message of  "Send raw transaction"
  3. Call the RPC method " web3_eth_sendRawTransaction " to send the request message to the blockchain
  4. Parse the received blockchain response message and return the analysis result

#### Protocol layer implementation of PlatONE
The implementation of PlatONE's protocol layer is almost the same as that of Ethereum. The only difference is that the data field of raw transaction is filled with one more transaction type encoding, and has one more transaction type field in the RLP process of raw transaction. Because the data field is filled by users who use BoAT SDK before calling BoAT SDK-related APIs, the protocol layer of PlatONE can reuse the protocol layer of Ethereum.


### RPC layer

#### Overview

The RPC layer implements the encapsulation of the specific link that sends data to the blockchain node,and provide services to the protocol layer.The RPC layer is an abstraction of the concrete realization of the sending link, so that the BoAT SDK can be transplanted to different operating environments more conveniently.

#### Scalable design of RPC layer
The RPC layer expands down to implement different sending links according to the specific operating environment. For example, some environments provide the transmission tool cURL, you can use curl to send and receive messages. Some environments do not support cURL, but provide AT commands to send and receive messages. The RPC layer encapsulates these different implementations. The layer provides a unified interface, so that the protocol layer does not need to care about the specific transmission link, and only needs to call the unified interface when sending and receiving messages.The RPC layer's encapsulation of different links also facilitates the expansion of more link implementations.

##### Extension of RPC layer to cURL
cURL is a file transfer tool that uses URL syntax to work under the command line. It supports multiple communication protocols such as FTP, FTPS, HTTP, and HTTPS. When the RPC layer enables cURL support, in addition to the global resource initialization of cURL when the SDK is initialized, it must also be implemented at the RPC layer:
   1. cURL session initialization
   2. Set the URL format to\<protocol\>://<target name | IP>:\<port\>
   3. Configure the protocol supported by cURL
   4. Configure SSL certificate (if needed)
   5. Set HTTP type to POST
   6. Set the timeout period (response timeout, connection timeout)
   7. Set HTTP header information
   8. Set the callback function of HTTP message response and receive buffer
   9. Set POST content
   10. Perform RPC request
   11. CURL resource cleaning and recycling  



### Hardware dependency layer
#### Overview
BoAT SDK will run on different hardware platforms. In order to make better use of hardware resources, BoAT SDK provides a hardware dependency layer.The hardware dependency layer provides services for the wallet at the interface layer, providing services such as random number generators, secure storage,encryption and signature.Since the resources provided by different hardware platforms may be different, for example,some hardware provides the hardware implementation of the random number generator, some hardware platforms not only provide the hardware implementation of the random number generator, but also provide the TEE environment, which has hardware dependence Layer, BoAT SDK can make better use of these hardware resources.

#### Software implementation of the hardware dependent layer
BoAT SDK should provide a hardware-dependent full software implementation, so that BoAT can still run completely when the hardware cannot provide the required complete services.Hardware-dependent software implementation should be based on covering the necessary hardware services as a standard, and at least provide the following functional implementations:
+ Random number generator
+ Signature (such as ECDSA used by Ethereum)
+ Secure storage (such as storing private keys in an encrypted manner)
+ Hash algorithm (such as Keccak-256 used by Ethereum)


#### TEE support
The design of BoAT should consider the support of TEE environment.For hardware with a TEE environment, BoAT should be able to put sensitive information in the TEE environment with a small amount of modification.To meet this goal, the wallet is designed to meet the following criteria:
+ Independent design of wallet-related data structure
+ Wallet related implementation independent design
+ Sensitive information related to the wallet is not reflected outside the wallet

### General tool implementation

#### Overview
General tools exist independently of each layer and are used to generate C language interfaces for accessing blockchain smart contracts.General tools should be implemented in scripting languages.Common tools include:
+ Tool used to generate C language interface of Ethereum smart contract
+ Tool used to generate C language interface of PlatONE smart contract

#### Brief description of general tools

##### Tool for generating C language interface of Ethereum smart contract
The commonly used programming language for Ethereum smart contracts is solidity. After solidity is compiled, a JSON file describing the contract interface will be generated.The JSON format of the contract interface is given by an array of function and/or event descriptions.The description of a function is a JSON object with the following fields:
+ type: "function" , "constructor" , "receive" (Function to receive Ether) or "fallback"( Default function)
+ name: Function name
+ inputs :Object array, each array object will contain:
   - name: parameter name；
   - type: Authoritative type of parameter 
   - components: For tuple type
+ outputs: An array of objects similar to inputs, which can be omitted if the function has no return value
+ payable: If the function accepts Ether, it is true; the default is false
+ stateMutability: One of the following values: pure (specified not to read the state of the blockchain), view (specified not to modify the state of the blockchain), nonpayable and payable (same as payable above)
+ constant: True if the function is specified as pure or view  

For the generated C language contract interface, the corresponding relationship with the JSON object is as follows:
<table>
    <tr>
        <td colspan="2">JSON object</td>
        <td>JSON object value</td>
        <td>C language contract interface</td>
    </tr>
    <tr>
        <td colspan="2">Type</td>
        <td>/</td>
        <td>/</td>
    </tr>
    <tr>
        <td colspan="2">Name</td>
        <td>/</td>
        <td>Function name</td>
    </tr>
    <tr>
        <td rowspan="3">Inputs</td>
        <td>Name</td>
        <td>/</td>
        <td>Function parameter name</td>
    </tr>
     <tr>
        <td>Type</td>
        <td>/</td>
        <td>Function parameter type</td>
    </tr>
     <tr>
        <td>components</td>
        <td>/</td>
        <td>/</td>
    </tr>
      <tr>
        <td colspan="2">Outputs</td>
        <td>/</td>
        <td>The function return value, because the contract returns an http message, so the function should be processed as char*</td>
    </tr>
     <tr>
        <td colspan="2">Payable</td>
        <td>/</td>
        <td>/</td>
    </tr>
     <tr>
        <td colspan="2">stateMutability</td>
        <td>/</td>
        <td>/</td>
    </tr>
    <tr>
        <td colspan="2" rowspan="2">Constant</td>
        <td>True</td>
        <td>The function accesses the blockchain through the RPC method "web3_eth_call"</td>
    </tr>
    <tr>
        <td>False</td>
        <td>The function accesses the blockchain through the RPC method " web3_eth_sendRawTransaction "</td>
    </tr>
</table>

##### Tool for generating C language interface of PlatONE  smart contract
The commonly used PlatONE smart contract development language is C++. Like Ethereum, the PlatONE smart contract will also generate a JSON file describing the contract interface after being compiled. Its JSON field is the same as Ethereum's JSON field, and the correspondence between C language interface and JSON field is also consistent with Ethereum.


### Application

#### Overview
In the implementation of each layer of The BoAT SDK, more or less it involves the conversion of data formats, the codec of messages, and so on.These functions should be abstracted into independent modules to provide services for each layer. These functional blocks include:
+ Data format conversion tool
+ RLP encoding
+ JSON encoding and decoding

#### Data format conversion tool
In the design of BoAT SDK, data format conversion is used in many places, such as converting the input ASCII code into a binary format, and converting the binary format data returned by the blockchain into an ASCII code format that is convenient for display. The Little-endian and big endian conversions involved in the contract ABI interface, etc. For ease of use, it is advisable to put the format conversion function implementations that may be used in the same file.The data conversions that may be used are:
+ Convert binary stream to hexadecimal string
+ Convert a hexadecimal string to a binary stream
+ Convert uint32 type data to big endian
+ Convert uint64 type data to big endian
+ convert byte order of a byte stream  

In addition, in order to adapt the SDK to more environments, you can also encapsulate the following functions into this file:  
+ Heap memory application
+ Heap memory release
+ Sleep waiting  

#### RLP encoding

##### Structure of RLP
RLP encoding is used in two places. One is that the protocol layer organizes transaction messages to use RLP encoding, and the other is that RLP encoding may be used in the generated C language contract interface code.  
The definition of RLP encoding only handles two types of data: one is a string and the other is a list. String refer to a string of binary data, such as a byte array; List is a nested recursive structure, which can contain strings and lists, and its structure is shown in Figure 4-3:
![The structure of the RLP list](/en-us/images/BoAT_Overall_Design_en-F4-3-Structure_Of_RLP.png)  
Figure 4-3 The structure of the RLP list

##### RLP encoding rules
The encoding rules of RLP are described as follows:
+ If a string is 0-55 bytes long, the RLP encoding consists of a single byte with value 0x80 plus the length of the string followed by the string. The range of the first byte is thus [0x80, 0xb7].
+ If a string is more than 55 bytes long, the RLP encoding consists of a single byte with value 0xb7 plus the length in bytes of the length of the string in binary form, followed by the length of the string, followed by the string.
+ If the total payload of a list (i.e. the combined length of all its items being RLP encoded) is 0-55 bytes long, the RLP encoding consists of a single byte with value 0xc0 plus the length of the list followed by the concatenation of the RLP encodings of the items. The range of the first byte is thus [0xc0, 0xf7].
+ If the total payload of a list is more than 55 bytes long, the RLP encoding consists of a single byte with value 0xf7 plus the length in bytes of the length of the payload in binary form, followed by the length of the payload, followed by the concatenation of the RLP encodings of the items. The range of the first byte is thus [0xf8, 0xff].  

For a more detailed description of RLP encoding rules, please refer to the reference document Rlp wiki: https://eth.wiki/en/fundamentals/rlp


##### RLP encoding implementation
RLP encoding can be implemented in many different ways. As can be seen from the foregoing chapters, a possible data structure composition description of RLP encoding is shown in Figure 4-4:
![A possible data structure of RLP encoding](/en-us/images/BoAT_Overall_Design_en-F4-4-Data_Structure_Of_RLP.png)  
Figure 4-4 A possible data structure of RLP encoding

The figure defines four types to express the nested recursive structure of the RLP list. If there is a list object named List, it contains three string objects: stringA, stringB, stringC,Then a possible process of performing RLP encoding on the list object List is described as follows:
1. Initialize the list object List
2. Initialize the string object StringA
3. Add StringA to the list object List
4. Initialize the string object StringB
5. Add StringB to the list object List
6. Initialize the string object StringC
7. Add StringC to the list object List
8. Perform RLP encoding on List  

In summary, the implementation method of RLP should include at least:
+ RLP list initialization
+ RLP string initialization
+ Add string or list to a list
+ Perform RLP encoding on a string
+ Perform RLP encoding on the list
+ Replace a string in the list
+ Delete a string in the list
Calculate RLP code length

#### JSON encoding and decoding
In the message that BoAT SDK accesses the blockchain, the encoding of JSON will be involved, and the response message of the blockchain to the BoAT SDK will involve the decoding of JSON. The encoding and decoding of JSON in the message can be implemented with a three-party library: cJSON.  
cJSON is a lightweight JSON codec written in C language. It follows the ANSI-C standard and can adapt to more platforms and compilers. It is also very convenient to port cJSON into the SDK. Simply copy cJSON.h and cJSON.c to the SDK, and include the header file "cJSON.h" where you need to use cJSON to use it. For more description of cJSON, please refer to the reference document cJSON<https://github.com/DaveGamble/cJSON#welcome-to-cjson>.

## The process of creating a blockchain transaction using BoAT

### The process of creating an Ethereum transaction using BoAT
A typical process of using BoAT to create an Ethereum transaction is shown in Figure 5-1:  
![The process of creating a transaction using BoAT](/en-us/images/BoAT_Overall_Design_en-F5-1-Create_Ttransaction.png)  
Figure 5-1 The process of creating a transaction using BoAT

among them:
+ BoAT SDK initialization:
Please refer to the description of "BoAT SDK Initialization" in [SDK Initialization/Deinitialization](#SDK-Initialization-de-initialization)
+ Create a wallet  
Please refer to the description of "Create Wallet" in [Wallet Operation](#Wallet-Operation)
+ Transaction initialization  
Please refer to the description of "transaction initialization" in [Ethereum transaction interface function implementation brief description](#Brief-description-of-Ethereum-transaction-interface-function-implementation)
+ Set the Nonce field
+ Set the Data field
+ Send transaction  
Please refer to the description of "send transaction" in [Ethereum transaction interface function implementation brief description](#Brief-description-of-Ethereum-transaction-interface-function-implementation)
+ BoAT SDK de-initialization:  
Please refer to the description of "BoAT SDK de-initialization" in [SDK Initialization/Deinitialization](#SDK-Initialization-de-initialization)

### The process of creating an PlatONE transaction using BoAT
The process of creating a PlatONE transaction is similar to Ethereum. In addition to setting the Nonce field and the Data field, PlatONE also needs to set the transaction type field before sending the transaction. The rest of the process is consistent with Ethereum. For related description, please refer to [Process of creating an Ethereum transaction using BoAT](#The-process-of-creating-an-Ethereum-transaction-using-BoAT)


## Reference
[1]. cJSON<https://github.com/DaveGamble/cJSON#welcome-to-cjson>  
[2]. cURL<https://curl.haxx.se/libcurl/>  
[3]. RLP wiki<https://eth.wiki/en/fundamentals/rlp>  
