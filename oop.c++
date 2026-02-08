#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// BASE CLASS: Product
// ============================================================================
class Product {
protected:
    int id;
    string name;
    
public:
    // Constructor
    Product(int product_id, string product_name) 
        : id(product_id), name(product_name) {}
    
    // Virtual function - can be overridden by child classes
    virtual void display() {
        cout << "Product #" << id << ": " << name << endl;
    }
    
    // Getter functions
    int getId() const { return id; }
    string getName() const { return name; }
    
    // Virtual destructor (good practice for inheritance)
    virtual ~Product() {}
};

// ============================================================================
// CHILD CLASS: DefectiveProduct (Inheritance example)
// ============================================================================
class DefectiveProduct : public Product {
private:
    string defectType;
    
public:
    DefectiveProduct(int id, string name, string defect)
        : Product(id, name), defectType(defect) {}
    
    // Override parent's display function
    void display() override {
        cout << "DEFECTIVE Product #" << id << ": " << name 
             << " (Defect: " << defectType << ")" << endl;
    }
    
    string getDefectType() const { return defectType; }
};

// ============================================================================
// CLASS: RobotArm (Encapsulation, Constructors, Methods)
// ============================================================================
class RobotArm {
private:
    int x, y;           // Position
    int speed;          // Movement speed
    bool isActive;      // Is robot running?
    string status;      // Current status message
    
public:
    // Constructor - initializes robot to safe state
    RobotArm() : x(0), y(0), speed(0), isActive(false), status("Idle") {
        cout << "[ROBOT] Robot arm initialized at position (0, 0)" << endl;
    }
    
    // Destructor - cleanup when robot is destroyed
    ~RobotArm() {
        if (isActive) {
            stop();
        }
        cout << "[ROBOT] Robot arm shut down safely" << endl;
    }
    
    // Start the robot
    void start() {
        isActive = true;
        status = "Running";
        cout << "[ROBOT] Robot arm started!" << endl;
    }
    
    // Stop the robot
    void stop() {
        isActive = false;
        speed = 0;
        status = "Stopped";
        cout << "[ROBOT] Robot arm stopped!" << endl;
    }
    
    // Emergency stop
    void emergencyStop() {
        isActive = false;
        speed = 0;
        status = "EMERGENCY STOP";
        cout << "[ROBOT] !!! EMERGENCY STOP ACTIVATED !!!" << endl;
    }
    
    // Move to position (using references for efficiency)
    void moveTo(int new_x, int new_y) {
        if (!isActive) {
            cout << "[ROBOT] Cannot move - robot is not active!" << endl;
            return;
        }
        
        x = new_x;
        y = new_y;
        cout << "[ROBOT] Moved to position (" << x << ", " << y << ")" << endl;
    }
    
    // Set speed with validation
    void setSpeed(int new_speed) {
        if (new_speed < 0 || new_speed > 100) {
            cout << "[ROBOT] Invalid speed! Must be 0-100" << endl;
            return;
        }
        speed = new_speed;
        cout << "[ROBOT] Speed set to " << speed << "%" << endl;
    }
    
    // Pick up a product (passing by reference to avoid copying)
    void pickProduct(const Product& product) {
        if (!isActive) {
            cout << "[ROBOT] Cannot pick - robot is not active!" << endl;
            return;
        }
        cout << "[ROBOT] Picked up: ";
        product.display();
    }
    
    // Getters
    bool getIsActive() const { return isActive; }
    string getStatus() const { return status; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getSpeed() const { return speed; }
};

// ============================================================================
// CLASS: VisionSystem (Computer Vision Simulation)
// ============================================================================
class VisionSystem {
private:
    bool isCalibrated;
    int detectionCount;
    
public:
    VisionSystem() : isCalibrated(false), detectionCount(0) {
        cout << "[VISION] Vision system initialized" << endl;
    }
    
    void calibrate() {
        isCalibrated = true;
        cout << "[VISION] Camera calibrated successfully" << endl;
    }
    
    // Simulate detecting a product (returns true if product detected)
    bool detectProduct(string& detectedName) {
        if (!isCalibrated) {
            cout << "[VISION] Error: Camera not calibrated!" << endl;
            return false;
        }
        
        // Simulate detection
        detectionCount++;
        
        // Simulate random detection (in real version, this uses actual CV)
        if (detectionCount % 3 == 0) {
            detectedName = "Good Product";
            cout << "[VISION] Detected: Good Product" << endl;
            return true;
        } else if (detectionCount % 3 == 1) {
            detectedName = "Defective Product";
            cout << "[VISION] Detected: Defective Product (Scratched)" << endl;
            return true;
        } else {
            detectedName = "Unknown";
            cout << "[VISION] Detected: Unknown object (possibly hand)" << endl;
            return false;
        }
    }
    
    bool getIsCalibrated() const { return isCalibrated; }
    int getDetectionCount() const { return detectionCount; }
};

// ============================================================================
// CLASS: Dashboard (Main Control System)
// ============================================================================
class Dashboard {
private:
    RobotArm robot;
    VisionSystem vision;
    vector<Product*> goodProducts;      // Using pointers to store different types
    vector<DefectiveProduct*> badProducts;
    int totalProcessed;
    
public:
    Dashboard() : totalProcessed(0) {
        cout << "\n========================================" << endl;
        cout << "   ROBOT ARM CONTROL DASHBOARD v1.0" << endl;
        cout << "========================================\n" << endl;
    }
    
    ~Dashboard() {
        // Clean up dynamically allocated products
        for (auto product : goodProducts) {
            delete product;
        }
        for (auto product : badProducts) {
            delete product;
        }
        cout << "\n[DASHBOARD] Cleanup complete" << endl;
    }
    
    void initialize() {
        cout << "[DASHBOARD] Initializing system..." << endl;
        vision.calibrate();
        cout << "[DASHBOARD] System ready!" << endl;
    }
    
    void startProduction() {
        cout << "\n[DASHBOARD] Starting production..." << endl;
        robot.start();
        robot.setSpeed(50);
    }
    
    void processProduct() {
        string detectedName;
        
        if (vision.detectProduct(detectedName)) {
            totalProcessed++;
            
            if (detectedName == "Good Product") {
                // Create good product using 'new'
                Product* product = new Product(totalProcessed, "Widget-" + to_string(totalProcessed));
                goodProducts.push_back(product);
                
                robot.moveTo(100, 50);
                robot.pickProduct(*product);  // Dereference pointer to pass by reference
                robot.moveTo(200, 50);
                cout << "[DASHBOARD] Placed on good products board" << endl;
                
            } else if (detectedName == "Defective Product") {
                // Create defective product
                DefectiveProduct* product = new DefectiveProduct(
                    totalProcessed, 
                    "Defect-" + to_string(totalProcessed),
                    "Surface scratch"
                );
                badProducts.push_back(product);
                
                robot.moveTo(100, 50);
                robot.pickProduct(*product);
                robot.moveTo(200, 100);
                cout << "[DASHBOARD] Placed on defective products board" << endl;
            }
        } else {
            cout << "[DASHBOARD] Ignoring unknown object" << endl;
        }
    }
    
    void showStatistics() {
        cout << "\n========================================" << endl;
        cout << "         PRODUCTION STATISTICS" << endl;
        cout << "========================================" << endl;
        cout << "Total processed: " << totalProcessed << endl;
        cout << "Good products: " << goodProducts.size() << endl;
        cout << "Defective products: " << badProducts.size() << endl;
        cout << "Robot status: " << robot.getStatus() << endl;
        cout << "Robot position: (" << robot.getX() << ", " << robot.getY() << ")" << endl;
        cout << "Detections: " << vision.getDetectionCount() << endl;
        cout << "========================================\n" << endl;
    }
    
    void showGoodProducts() {
        cout << "\nGOOD PRODUCTS:" << endl;
        for (const auto& product : goodProducts) {
            product->display();
        }
    }
    
    void showDefectiveProducts() {
        cout << "\nDEFECTIVE PRODUCTS:" << endl;
        for (const auto& product : badProducts) {
            product->display();
        }
    }
    
    void shutdown() {
        cout << "\n[DASHBOARD] Shutting down system..." << endl;
        robot.stop();
    }
};

// ============================================================================
// MAIN FUNCTION - Simulates dashboard usage
// ============================================================================
int main() {
    // Create dashboard (automatically creates robot and vision)
    Dashboard dashboard;
    
    // Initialize
    dashboard.initialize();
    
    // Start production
    dashboard.startProduction();
    
    // Process some products (simulating production line)
    cout << "\n--- Processing products ---\n" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "\n--- Item " << (i+1) << " ---" << endl;
        dashboard.processProduct();
    }
    
    // Show statistics
    dashboard.showStatistics();
    
    // Show all products
    dashboard.showGoodProducts();
    dashboard.showDefectiveProducts();
    
    // Shutdown
    dashboard.shutdown();
    
    cout << "\n[SYSTEM] Program completed successfully" << endl;
    
    return 0;
}