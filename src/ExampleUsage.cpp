// ExampleUsage.cpp - Demonstrates how to use the enhanced Glitch SDK

#include "GlitchSDK.h"
#include <iostream>

class GameAnalyticsManager 
{
private:
    std::string authToken;
    std::string titleId;
    std::string installRecordId; // Store the install ID for purchases

public:
    GameAnalyticsManager(const std::string& token, const std::string& title) 
        : authToken(token), titleId(title) {}

    // Example 1: Basic install tracking (no fingerprinting)
    void RecordBasicInstall() 
    {
        std::string userInstallId = "unique_user_device_id_123";
        std::string platform = "steam"; // or "windows", "epic", etc.
        
        std::string response = GlitchSDK::CreateInstallRecord(
            authToken, 
            titleId, 
            userInstallId, 
            platform
        );
        
        std::cout << "Basic Install Response: " << response << std::endl;
    }

    // Example 2: Advanced install tracking with automatic fingerprinting
    void RecordInstallWithAutoFingerprint() 
    {
        std::string userInstallId = "unique_user_device_id_456";
        std::string platform = "steam";
        
        // Automatically collect system fingerprint
        GlitchSDK::FingerprintComponents fingerprint = GlitchSDK::CollectSystemFingerprint();
        
        // Optional: Add keyboard layout for better cross-device tracking
        fingerprint.KeyboardLayout = GlitchSDK::GenerateKeyboardLayout();
        
        // Optional: Add custom device model info
        fingerprint.DeviceModel = "Custom Gaming PC, RTX 4080";
        fingerprint.DeviceManufacturer = "Custom Build";
        
        std::string response = GlitchSDK::CreateInstallRecordWithFingerprint(
            authToken,
            titleId,
            userInstallId,
            platform,
            fingerprint,
            "1.2.3", // game version
            "steam_store" // referral source
        );
        
        std::cout << "Advanced Install Response: " << response << std::endl;
        
        // Extract install ID from response for future purchases
        // (In real usage, you'd parse the JSON response)
        installRecordId = "extracted_install_uuid_from_response";
    }

    // Example 3: Manual fingerprint creation with custom data
    void RecordInstallWithManualFingerprint() 
    {
        std::string userInstallId = "unique_user_device_id_789";
        std::string platform = "epic";
        
        // Manually create fingerprint components
        GlitchSDK::FingerprintComponents fingerprint;
        
        // Device info
        fingerprint.DeviceModel = "ASUS ROG Strix G15";
        fingerprint.DeviceType = "desktop"; // or "laptop" 
        fingerprint.DeviceManufacturer = "ASUS";
        
        // OS info
        fingerprint.OSName = "Windows";
        fingerprint.OSVersion = "11.0.22621";
        
        // Display
        fingerprint.DisplayResolution = "1920x1080";
        fingerprint.DisplayDensity = 96;
        
        // Hardware
        fingerprint.CPUModel = "AMD Ryzen 7 5800H (16-thread)";
        fingerprint.CPUCores = 8;
        fingerprint.GPUModel = "NVIDIA RTX 3070 Mobile 8GB";
        fingerprint.MemoryMB = 16384; // 16GB
        
        // Environment
        fingerprint.Language = "en-US";
        fingerprint.Timezone = "America/New_York";
        fingerprint.Region = "US";
        
        // Desktop-specific data
        fingerprint.FormFactors = {"Desktop"};
        fingerprint.Architecture = "x86";
        fingerprint.Bitness = "64";
        fingerprint.PlatformVersion = "10.0.22621";
        fingerprint.IsWow64 = false;
        
        // Custom keyboard layout (simplified example)
        fingerprint.KeyboardLayout = {
            {"KeyQ", "q"}, {"KeyW", "w"}, {"KeyE", "e"}, {"KeyR", "r"},
            {"KeyA", "a"}, {"KeyS", "s"}, {"KeyD", "d"}, {"KeyF", "f"},
            {"Digit1", "1"}, {"Digit2", "2"}, {"Semicolon", ";"}
            // ... add all required keys as per documentation
        };
        
        std::string response = GlitchSDK::CreateInstallRecordWithFingerprint(
            authToken,
            titleId,
            userInstallId,
            platform,
            fingerprint,
            "1.2.3",
            "epic_store"
        );
        
        std::cout << "Manual Fingerprint Install Response: " << response << std::endl;
    }

    // Example 4: Record a purchase/revenue event
    void RecordGamePurchase() 
    {
        if (installRecordId.empty()) {
            std::cout << "Error: No install record ID available for purchase" << std::endl;
            return;
        }
        
        // Create purchase data
        GlitchSDK::PurchaseData purchase(installRecordId);
        purchase.PurchaseType = "in_app";
        purchase.PurchaseAmount = 9.99f;
        purchase.Currency = "USD";
        purchase.TransactionID = "STEAM_TXN_ABC123456";
        purchase.ItemSKU = "premium_character_pack";
        purchase.ItemName = "Premium Character Bundle";
        purchase.Quantity = 1;
        purchase.MetadataJSON = R"({"promo_code":"NEWUSER20","discount":0.20})";
        
        std::string response = GlitchSDK::RecordPurchase(authToken, titleId, purchase);
        std::cout << "Purchase Response: " << response << std::endl;
    }

    // Example 5: Record multiple purchases (DLC, microtransactions, etc.)
    void RecordMultiplePurchases() 
    {
        if (installRecordId.empty()) {
            std::cout << "Error: No install record ID available for purchases" << std::endl;
            return;
        }
        
        // Purchase 1: DLC
        GlitchSDK::PurchaseData dlc(installRecordId);
        dlc.PurchaseType = "dlc";
        dlc.PurchaseAmount = 14.99f;
        dlc.Currency = "USD";
        dlc.TransactionID = "STEAM_DLC_XYZ789";
        dlc.ItemSKU = "expansion_pack_1";
        dlc.ItemName = "Arctic Adventure DLC";
        
        std::string dlcResponse = GlitchSDK::RecordPurchase(authToken, titleId, dlc);
        std::cout << "DLC Purchase Response: " << dlcResponse << std::endl;
        
        // Purchase 2: Microtransaction
        GlitchSDK::PurchaseData microtx(installRecordId);
        microtx.PurchaseType = "in_app";
        microtx.PurchaseAmount = 2.99f;
        microtx.Currency = "USD";
        microtx.TransactionID = "STEAM_MICRO_DEF456";
        microtx.ItemSKU = "coin_pack_500";
        microtx.ItemName = "500 Gold Coins";
        microtx.Quantity = 1;
        microtx.MetadataJSON = R"({"source":"in_game_store","category":"currency"})";
        
        std::string microResponse = GlitchSDK::RecordPurchase(authToken, titleId, microtx);
        std::cout << "Microtransaction Response: " << microResponse << std::endl;
    }

    // Example 6: Utility function to test fingerprint JSON generation
    void TestFingerprintJSON() 
    {
        GlitchSDK::FingerprintComponents test;
        test.DeviceModel = "Test Device";
        test.DeviceType = "desktop";
        test.OSName = "Windows";
        test.OSVersion = "11.0";
        test.CPUModel = "Test CPU";
        test.KeyboardLayout = {{"KeyQ", "q"}, {"KeyW", "w"}};
        
        std::string json = GlitchSDK::FingerprintToJSON(test);
        std::cout << "Generated Fingerprint JSON: " << json << std::endl;
    }
};

// Main usage examples
int main() 
{
    // Initialize with your credentials
    std::string authToken = "your_bearer_token_here";
    std::string titleId = "your_title_uuid_here";
    
    GameAnalyticsManager analytics(authToken, titleId);
    
    std::cout << "=== Glitch SDK Examples ===" << std::endl;
    
    // Example 1: Basic tracking
    std::cout << "\n1. Basic Install Tracking:" << std::endl;
    analytics.RecordBasicInstall();
    
    // Example 2: Advanced tracking with auto-fingerprinting
    std::cout << "\n2. Advanced Install with Auto-Fingerprinting:" << std::endl;
    analytics.RecordInstallWithAutoFingerprint();
    
    // Example 3: Manual fingerprinting
    std::cout << "\n3. Manual Fingerprint Creation:" << std::endl;
    analytics.RecordInstallWithManualFingerprint();
    
    // Example 4: Purchase tracking
    std::cout << "\n4. Purchase Tracking:" << std::endl;
    analytics.RecordGamePurchase();
    
    // Example 5: Multiple purchases
    std::cout << "\n5. Multiple Purchase Types:" << std::endl;
    analytics.RecordMultiplePurchases();
    
    // Example 6: JSON testing
    std::cout << "\n6. Fingerprint JSON Test:" << std::endl;
    analytics.TestFingerprintJSON();
    
    return 0;
}
