# Unreal Engine - Glitch Gaming API Integration

This repository contains a comprehensive C++ SDK for integrating with the Glitch Gaming analytics platform from within your Unreal Engine projects. The SDK supports install tracking, cross-platform user fingerprinting, and revenue/purchase recording.

## Overview

The Glitch SDK enables you to:
- **Track game installs** with basic or advanced fingerprinting
- **Cross-platform user identification** to understand user journeys across devices 
- **Record purchases and revenue events** for LTV analysis
- **Automatic system fingerprinting** with minimal developer effort

## Features

### ✅ Core Functionality
- **Basic Install Tracking**: Record game installations with minimal data
- **Advanced Fingerprinting**: Comprehensive device fingerprinting for cross-device tracking
- **Purchase/Revenue Tracking**: Record in-app purchases, DLC sales, and other revenue events
- **Automatic Data Collection**: Built-in system information gathering

### ✅ Cross-Platform Support
- Windows (with advanced fingerprinting)
- macOS (with system information collection)
- Linux (with basic system information)

### ✅ Privacy Compliant
- All fingerprinting is **optional**
- No personally identifiable information (PII) collected
- Respects user privacy while enabling analytics

## File Structure

```
/src/
├── GlitchSDK.h              # Main SDK header with all declarations
├── GlitchSDK.cpp            # Complete implementation
└── ExampleUsage.cpp         # Comprehensive usage examples

/README.md                   # This documentation file
```

## Installation

1. **Clone or download** this repository into your Unreal project's `Source` directory
2. **Add dependencies** to your `<ProjectName>.Build.cs` file:
   ```csharp
   PublicDependencyModuleNames.AddRange(new string[] { 
       "Core", "HTTP", "Json", "JsonUtilities" 
   });
   
   // Enable libcurl (usually available by default in UE)
   bEnableUndefinedIdentifierWarnings = false;
   ```
3. **Include the header** in your C++ files:
   ```cpp
   #include "GlitchSDK.h"
   ```

## Quick Start

### Basic Install Tracking
```cpp
#include "GlitchSDK.h"

// Record a simple install
std::string response = GlitchSDK::CreateInstallRecord(
    "YOUR_AUTH_TOKEN",
    "YOUR_TITLE_UUID", 
    "unique_user_device_id",
    "steam"  // platform
);
```

### Advanced Install Tracking with Auto-Fingerprinting
```cpp
// Automatically collect system fingerprint
GlitchSDK::FingerprintComponents fingerprint = GlitchSDK::CollectSystemFingerprint();

// Optional: Add keyboard layout for better cross-device matching
fingerprint.KeyboardLayout = GlitchSDK::GenerateKeyboardLayout();

// Record install with fingerprinting
std::string response = GlitchSDK::CreateInstallRecordWithFingerprint(
    "YOUR_AUTH_TOKEN",
    "YOUR_TITLE_UUID",
    "unique_user_device_id", 
    "steam",
    fingerprint,
    "1.2.3",        // game version (optional)
    "steam_store"   // referral source (optional)
);
```

### Purchase/Revenue Tracking
```cpp
// Create purchase data
GlitchSDK::PurchaseData purchase("INSTALL_UUID_FROM_PREVIOUS_CALL");
purchase.PurchaseType = "in_app";
purchase.PurchaseAmount = 9.99f;
purchase.Currency = "USD";
purchase.ItemSKU = "premium_pack";
purchase.ItemName = "Premium Character Pack";

// Record the purchase
std::string response = GlitchSDK::RecordPurchase(
    "YOUR_AUTH_TOKEN",
    "YOUR_TITLE_UUID", 
    purchase
);
```

## API Reference

### Core Functions

#### `CreateInstallRecord`
Records a basic game installation.
```cpp
std::string CreateInstallRecord(
    const std::string& authToken,    // Your API bearer token
    const std::string& titleId,      // UUID of your game title  
    const std::string& userInstallId,// Unique user/device ID
    const std::string& platform      // Platform: "steam", "epic", etc.
);
```

#### `CreateInstallRecordWithFingerprint` 
Records an installation with comprehensive device fingerprinting.
```cpp
std::string CreateInstallRecordWithFingerprint(
    const std::string& authToken,
    const std::string& titleId,
    const std::string& userInstallId, 
    const std::string& platform,
    const FingerprintComponents& fingerprint,  // Device fingerprint data
    const std::string& gameVersion = "",       // Optional game version
    const std::string& referralSource = ""     // Optional referral source
);
```

#### `RecordPurchase`
Records a purchase or revenue event.
```cpp
std::string RecordPurchase(
    const std::string& authToken,
    const std::string& titleId,
    const PurchaseData& purchaseData    // Purchase information
);
```

### Utility Functions

#### `CollectSystemFingerprint`
Automatically collects available system information for fingerprinting.
```cpp
FingerprintComponents CollectSystemFingerprint();
```

#### `GenerateKeyboardLayout` 
Generates a keyboard layout map for enhanced cross-device tracking.
```cpp
std::map<std::string, std::string> GenerateKeyboardLayout();
```

## Data Structures

### `FingerprintComponents`
Contains optional device characteristics for cross-platform user identification:

```cpp
struct FingerprintComponents {
    // Device info  
    std::string DeviceModel;        // e.g., "Dell XPS 15, Intel i7, RTX 3060"
    std::string DeviceType;         // "desktop", "mobile", "console" 
    std::string DeviceManufacturer; // e.g., "Dell", "NVIDIA"
    
    // OS info
    std::string OSName;             // e.g., "Windows", "Linux"
    std::string OSVersion;          // e.g., "11.0.22621"
    
    // Hardware info
    std::string CPUModel;           // e.g., "Intel i7 12700H (14-core)"
    int CPUCores;                   // Physical core count
    std::string GPUModel;           // e.g., "NVIDIA RTX 3060 6GB" 
    int MemoryMB;                   // Total RAM in MB
    
    // Display info
    std::string DisplayResolution;  // e.g., "1920x1080"
    int DisplayDensity;             // DPI value
    
    // Environment
    std::string Language;           // e.g., "en-US"
    std::string Timezone;           // e.g., "America/New_York"
    
    // Desktop-specific (for PC platforms)
    std::vector<std::string> FormFactors; // e.g., {"Desktop"}
    std::string Architecture;       // e.g., "x86"  
    std::string Bitness;           // e.g., "64"
    
    // Keyboard layout for cross-device matching
    std::map<std::string, std::string> KeyboardLayout;
};
```

### `PurchaseData`
Contains purchase/revenue information:

```cpp
struct PurchaseData {
    std::string GameInstallID;      // Required: UUID from install record
    std::string PurchaseType;       // e.g., "in_app", "dlc", "ad_revenue"
    float PurchaseAmount;           // Monetary value
    std::string Currency;           // e.g., "USD", "EUR"
    std::string TransactionID;      // External transaction reference
    std::string ItemSKU;            // Product identifier
    std::string ItemName;           // Human-readable product name
    int Quantity;                   // Number of units (default: 1)
    std::string MetadataJSON;       // Additional data as JSON string
};
```

## Platform-Specific Features

### Windows
- Automatic CPU model detection via `__cpuid`
- Memory information via Windows APIs
- Display resolution detection
- Keyboard layout mapping (basic implementation)
- Desktop user agent client hints simulation

### macOS  
- CPU model detection via `sysctlbyname`
- Physical core count detection
- Memory size detection via system APIs
- Basic system information collection

### Linux
- CPU information from `/proc/cpuinfo`
- Memory information from `/proc/meminfo`  
- Basic system specification gathering

## Privacy & Compliance

### What We Collect (Optional)
- Device model and manufacturer
- Operating system name and version
- Hardware specifications (CPU, RAM, GPU)
- Display resolution and density
- System language and timezone
- Keyboard layout mapping

### What We DON'T Collect
- Serial numbers or device-specific identifiers
- User names or personal information  
- File system information or installed applications
- Network configuration details
- Any personally identifiable information (PII)

### User Control
- **All fingerprinting is optional** - use basic tracking if preferred
- **Granular control** - include only the data you want to share
- **Transparent** - all collected data is clearly documented

## Error Handling

All SDK functions return response strings that should be checked:

```cpp
std::string response = GlitchSDK::CreateInstallRecord(/*...*/);

// Check for errors
if (response.find("CURL error:") != std::string::npos) {
    // Handle network error
    UE_LOG(LogTemp, Error, TEXT("Network error: %s"), *FString(response.c_str()));
} else if (response.find("error") != std::string::npos) {
    // Handle API error  
    UE_LOG(LogTemp, Warning, TEXT("API error: %s"), *FString(response.c_str()));
} else {
    // Success - parse JSON response if needed
    UE_LOG(LogTemp, Log, TEXT("Success: %s"), *FString(response.c_str()));
}
```

## Best Practices

### Install Tracking
1. **Call once per installation** - not on every game launch
2. **Use persistent user IDs** - maintain the same ID across app updates  
3. **Include fingerprinting** for better cross-device attribution
4. **Store install record ID** from the response for purchase tracking

### Fingerprinting  
1. **Use `CollectSystemFingerprint()`** for automatic data collection
2. **Add keyboard layout** with `GenerateKeyboardLayout()` for better matching
3. **Customize device model** with meaningful descriptions
4. **Respect user privacy** - only collect what you need

### Purchase Tracking
1. **Link to install records** - always provide a valid `GameInstallID`
2. **Include transaction IDs** to prevent duplicate recordings
3. **Use meaningful SKUs** for product analysis
4. **Add metadata** for advanced analytics

## Examples

See `ExampleUsage.cpp` for comprehensive examples including:
- Basic install tracking
- Advanced fingerprinting  
- Manual fingerprint creation
- Single and multiple purchase recording
- Error handling patterns

## Dependencies

- **libcurl**: HTTP client library (usually included with Unreal Engine)
- **Standard C++11**: No additional C++ libraries required
- **Unreal Engine 4.25+**: Tested with UE 4.25 and later

## Support

- **Documentation**: See the main Glitch API documentation
- **Issues**: Open GitHub issues for bugs or feature requests
- **Community**: Join the Glitch Gaming Discord for support

## License

This SDK is provided under the MIT License. Feel free to use it in your commercial and non-commercial Unreal Engine projects.

---

**Need help?** Check out `ExampleUsage.cpp` for detailed implementation examples, or visit our developer documentation at [docs.glitch.fun](https://docs.glitch.fun).
