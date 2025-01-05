# Unreal Engine - Glitch Gaming API Integration

This repository contains an example **C++** function that uses **libcurl** to send a POST request to the Glitch API. The code is located in the `/src/CreateInstallRecord.cpp` file.

## Overview

Call this function within your Unreal Engine project to record a new install. It makes an HTTP POST to `https://api.glitch.fun/api/titles/{titleId}/installs`.

## File Information

- **File Name**: `CreateInstallRecord.cpp`
- **Location**: `/src/CreateInstallRecord.cpp`
- **Language**: C++
- **Library**: Uses **libcurl** for HTTP requests

## Installation

1. **Clone or copy** the repository into your Unreal project’s `Source` folder (or wherever you keep your C++ code).
2. Make sure **libcurl** is available. (Unreal Engine generally bundles libcurl on most platforms, but check your build settings.)
3. Add an include reference in your Unreal `Build.cs` if needed:
```csharp
   // In <ProjectName>.Build.cs
   PublicDependencyModuleNames.AddRange(new string[] { "HTTP", "Json", "JsonUtilities" });
   // Or ensure your environment has libcurl installed
```

### Usage
In your Unreal code, include the header that declares CreateInstallRecord(...).


Call the function:

```csharp
#include "CreateInstallRecord.h"

// ...
std::string response = CreateInstallRecord(
    "YOUR_AUTH_TOKEN",
    "TITLE_UUID",
    "DEVICE_UNIQUE_ID",
    "steam"
);
```

Check the returned response string for success or error details.


## Example


```csharp
void UMyInstallHandler::RecordInstall()
{
    std::string response = CreateInstallRecord(
        "MySecretBearerToken",
        "abc123-uuid-of-title",
        "device-unique-id-xyz",
        "steam"
    );
    
    UE_LOG(LogTemp, Log, TEXT("Response: %s"), *FString(response.c_str()));
}
```

### Contributing
Contributions (via pull requests) are welcome. If you encounter any issues with cURL or compilation, please open an issue.

### License
This code is provided under the MIT License. Feel free to use it in your Unreal projects.
