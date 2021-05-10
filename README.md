# DSS_NCP_TAKE_HOME_EXERCISE_V5_improved
Disney Streaming Services - Connected Device Native Client Engineering
* Create a screen that consumes Disney Plus home page API and renders the pre-populated data onto the screen
* Navigation Support
* ------------------ Mouse input -- for debug. 
* ------------------ Similar to a remote control -- up/down/left/right/enter/back/etc 
* Layout 
* ------ for simplicity, creates n * n grids based on the screen widh & height
* ------ it can be multiple rows of data
* ------ grids can scale up to fill the entire screen
* 
* Special Notes
* ----------------- can dynamically populate the 'ref' sets as they come into view
* ----------------- Allow interaction or selection of a tile
* ----------------- Incorporate transition or visual aesthetics
* ----------------- Resize is disabled for demo 

# Navigation Keys
* ---------------- Left   scroll left
* ---------------- Right  scroll right
* ---------------- Up     scroll Up
* ---------------- Down   scroll Down
* ---------------- Enter  displays selected movie in a new window
* ---------------- back   returns to main window
* ---------------- esc    exit the application


# A note on backward-compatibility
* The application is built with C++11, C++14, and C++17
* can't be used with older compilers

# dependencies
* libcurl   
* opengl          
* freeglut       
* jsoncpp
* stb

# Building Application in Windows
* download DSS_NCP_TAK_HOME_EXERCISE_V5_improved
* Open The Project in Microsft Visual C++ 
* Build and run

# Building Application in Mac         
* coming soon
