//
//  ConstLoadTests.cpp
//  GSAP
//
//  Created by Teubert, Christopher (ARC-TI) on 2/14/18.
//
//

#include "LoadTests.hpp"
#include "Test.h"
#include "ConstLoadEstimator.h"
#include "LoadEstimatorFactory.h"
#include "GSAPConfigMap.h"

using namespace PCOE::Test;

namespace PCOE {
    void LoadTestInit() {

    }

    
    void testConstLoad() {
        GSAPConfigMap configMap;
        
        try {
            ConstLoadEstimator c = ConstLoadEstimator(configMap);
            Assert::Fail("Accepted missiong loading key");
        } catch (...) {
        }
        
        LoadEstimate test = {1, 2, 3};
        std::vector<std::string> testStr;
        for (auto && testElement : test) {
            testStr.push_back(std::to_string(testElement));
        }
        configMap[ConstLoadEstimator::LOADING_KEY] = testStr;
        
        ConstLoadEstimator c = ConstLoadEstimator(configMap);
        
        LoadEstimate test2 = c.estimateLoad(NAN, 0);
        
        Assert::AreEqual(test, test2, "Sampling not correct");
        
        configMap[ConstLoadEstimator::LOADING_KEY] = {};
        
        ConstLoadEstimator c2 = ConstLoadEstimator(configMap);
        
        LoadEstimate test3 = c2.estimateLoad(NAN, NAN);
        Assert::IsTrue(test3.empty(), "Empty loading vector sampling doesn't return empty sample");
    }
    
    void testConstLoadWithUncert() {
        GSAPConfigMap configMap;

        LoadEstimate test = {1, 2, 3};
        LoadEstimate std = {0.1, 0.1, 0.1};
        std::vector<std::string> testStr, stdStr;
        for (auto && testElement : test) {
            testStr.push_back(std::to_string(testElement));
        }
        for (auto && stdElement: std) {
            stdStr.push_back(std::to_string(stdElement));
        }
        configMap[ConstLoadEstimator::LOADING_KEY] = testStr;
        configMap[ConstLoadEstimator::STDDEV_KEY] = stdStr;
        ConstLoadEstimator c = ConstLoadEstimator(configMap);
        Assert::AreEqual(c.getUncertaintyMode(), ConstLoadEstimator::GAUSSIAN);
        
        LoadEstimate test2 = c.estimateLoad(NAN, 0);
        size_t nSame = 0;
        for (size_t i = 0; i < test.size(); i++) {
            Assert::IsTrue(test2[i] > test[i] - 1 && test2[i] < test[i] + 1); // EXTREMELY UNLIKELY TO FAIL
            if (test2[i] == test[i]) {
                nSame++;
            }
        }
        Assert::IsTrue(nSame < test.size(), "Data with "); // Extremely unlikely to fail
        
        // Test unequal numbers
        configMap[ConstLoadEstimator::LOADING_KEY].push_back("4");
        ConstLoadEstimator c2 = ConstLoadEstimator(configMap);
        Assert::AreEqual(c2.getUncertaintyMode(), ConstLoadEstimator::NONE, "Did not revert to no uncertainty when given unequal mean and std vector lengths");
    }
    
    void testFactory() {
        GSAPConfigMap configMap;
        LoadEstimate test = {1, 2, 3};
        std::vector<std::string> testStr;
        for (auto && testElement : test) {
            testStr.push_back(std::to_string(testElement));
        }
        configMap.insert(std::pair<std::string, std::vector<std::string> >(ConstLoadEstimator::LOADING_KEY, testStr));
        
        LoadEstimatorFactory &f = LoadEstimatorFactory::instance();
        std::unique_ptr<LoadEstimator> c = std::unique_ptr<LoadEstimator>(f.Create("const", configMap));

    }
}
