#include "Analysis.hh"

/**
 * Setting the Singleton Class
 */
Analysis* Analysis::instance=0;
/**
 * Instance of the Singleton Class
 *
 * @return the instance of the Singleton Materials
 */
Analysis* Analysis::GetInstance(){
    if (instance ==0){
        instance = new Analysis();
    }
    return instance;
}