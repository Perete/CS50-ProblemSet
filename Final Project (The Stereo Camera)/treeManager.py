import os 

def checkFolder(pathToFolder):
    folderExist = os.path.exists(pathToFolder)

    if not folderExist:
        # Create the folder
        os.mkdir(pathToFolder)
        return False
    else:
        return True

def checkCameraConfigFile(pathToFile):
    
    fileExist = os.path.isfile(pathToFile)

    if fileExist:
        print("Config File Detected!")
        configValues = {}
        with open(pathToFile, 'r') as file:
            lines = file.readlines()

        for line in lines:
            # Remove blank spaces
            line.strip()
            # Split line by :
            key, value = line.split(':')
            # Put data in dict
            configValues[key] = int(value)
    
        return configValues
    else:
        print("Config File NOT Detected")
        return False

# For testing functions
if __name__ == "__main__":
    THIS_PATH = os.path.dirname(__file__)
    pathToFile = f"{THIS_PATH}/cameraConfig.txt"
    checkCameraConfigFile(pathToFile)