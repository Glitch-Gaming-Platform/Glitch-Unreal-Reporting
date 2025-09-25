#pragma once

#include <curl/curl.h>
#include <string>
#include <map>
#include <vector>

/**
 * Glitch Gaming API Integration for Unreal Engine
 * 
 * This SDK provides functions to integrate with the Glitch Gaming analytics platform,
 * including install tracking, fingerprinting, and purchase recording.
 */

namespace GlitchSDK 
{
    // Forward declarations
    struct FingerprintComponents;
    struct PurchaseData;
    
    /**
     * Device fingerprint components for cross-platform user tracking
     * All fields are optional - provide what's available on your platform
     */
    struct FingerprintComponents 
    {
        // Device information
        std::string DeviceModel;        // e.g., "Dell XPS 15, Intel i7, RTX 3060"
        std::string DeviceType;         // "desktop", "mobile", "console", etc.
        std::string DeviceManufacturer; // e.g., "Dell", "NVIDIA"
        
        // Operating System
        std::string OSName;             // e.g., "Windows", "Linux"
        std::string OSVersion;          // e.g., "10.0.22621"
        
        // Display
        std::string DisplayResolution;  // e.g., "1920x1080"
        int DisplayDensity = 0;         // DPI, e.g., 96, 144
        
        // Hardware
        std::string CPUModel;           // e.g., "Intel i7 12700H (14-core)"
        int CPUCores = 0;               // Physical cores
        std::string GPUModel;           // e.g., "NVIDIA RTX 3060 6GB"
        int MemoryMB = 0;               // Total RAM in MB
        
        // Environment
        std::string Language;           // e.g., "en-US"
        std::string Timezone;           // e.g., "America/New_York"
        std::string Region;             // e.g., "US"
        
        // Desktop-specific (for PC platforms)
        std::vector<std::string> FormFactors; // e.g., {"Desktop"}
        std::string Architecture;       // e.g., "x86"
        std::string Bitness;           // e.g., "64"
        std::string PlatformVersion;   // e.g., "10.0.22621"
        bool IsWow64 = false;          // 32-bit process on 64-bit OS
        
        // Keyboard layout (highly recommended for cross-device tracking)
        std::map<std::string, std::string> KeyboardLayout;
        
        // Identifiers
        std::string AdvertisingID;     // IDFA/AAID if available
        
        FingerprintComponents() = default;
    };
    
    /**
     * Purchase/revenue data for tracking sales and LTV
     */
    struct PurchaseData 
    {
        std::string GameInstallID;      // Required: UUID of existing install
        std::string PurchaseType;       // e.g., "in_app", "ad_revenue", "crypto"
        float PurchaseAmount = 0.0f;    // Monetary value
        std::string Currency;           // e.g., "USD", "EUR"
        std::string TransactionID;      // 3rd-party transaction ID
        std::string ItemSKU;            // Product SKU
        std::string ItemName;           // Human-readable product name
        int Quantity = 1;               // Number of units
        std::string MetadataJSON;       // Additional data as JSON string
        
        PurchaseData() = default;
        PurchaseData(const std::string& installID) : GameInstallID(installID) {}
    };
    
    /**
     * Core Functions
     */
    
    /**
     * Create a basic install record
     * @param authToken Bearer token for authentication
     * @param titleId UUID of the title/game
     * @param userInstallId Unique persistent user/device identifier
     * @param platform Platform identifier (steam, apple, android, etc.)
     * @return Response string from the API
     */
    std::string CreateInstallRecord(
        const std::string& authToken,
        const std::string& titleId, 
        const std::string& userInstallId,
        const std::string& platform
    );
    
    /**
     * Create an install record with fingerprinting data
     * @param authToken Bearer token for authentication
     * @param titleId UUID of the title/game
     * @param userInstallId Unique persistent user/device identifier
     * @param platform Platform identifier
     * @param fingerprint Device fingerprint components
     * @param gameVersion Optional game version string
     * @param referralSource Optional referral source
     * @return Response string from the API
     */
    std::string CreateInstallRecordWithFingerprint(
        const std::string& authToken,
        const std::string& titleId,
        const std::string& userInstallId,
        const std::string& platform,
        const FingerprintComponents& fingerprint,
        const std::string& gameVersion = "",
        const std::string& referralSource = ""
    );
    
    /**
     * Record a purchase/revenue event
     * @param authToken Bearer token for authentication
     * @param titleId UUID of the title/game
     * @param purchaseData Purchase information
     * @return Response string from the API
     */
    std::string RecordPurchase(
        const std::string& authToken,
        const std::string& titleId,
        const PurchaseData& purchaseData
    );
    
    /**
     * Utility Functions
     */
    
    /**
     * Auto-collect system fingerprint components where possible
     * This function attempts to gather device information automatically
     * @return FingerprintComponents with available system data
     */
    FingerprintComponents CollectSystemFingerprint();
    
    /**
     * Generate a canonical keyboard layout map for fingerprinting
     * @return Map of key codes to characters based on current system layout
     */
    std::map<std::string, std::string> GenerateKeyboardLayout();
    
    /**
     * Helper to convert fingerprint components to JSON
     * @param fingerprint Components to convert
     * @return JSON string representation
     */
    std::string FingerprintToJSON(const FingerprintComponents& fingerprint);
    
    /**
     * Helper to convert purchase data to JSON
     * @param purchase Purchase data to convert
     * @return JSON string representation
     */
    std::string PurchaseToJSON(const PurchaseData& purchase);

    // Internal helper functions
    namespace Internal 
    {
        size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
        std::string EscapeJSON(const std::string& input);
        std::string GetSystemInfo(const std::string& key);
    }
}
