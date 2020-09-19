/**
 * @file SpreaderDetectorBackend.c
 * @author Ron Shuvy
 * @id 206330193
 *
 * @brief This programs :
 *         1. identifies potential infection chains from an input (list of people and their meetings with each other).
 *         2. estimates the chance for each person ("contact") to be infected.
 *         3. delivers medical instructions for each person according to severity.
 */
// ------------------------------ includes ------------------------------
#include "SpreaderDetectorParams.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// -------------------------- const definitions -------------------------
#define FAILURE -1
#define DEFAULT_RISK 0
#define SICK 1
#define ARR_SIZE_MULTIPLIER 2
#define NUM_OF_CMD_ARGS 3
#define INITIAL_ARR_CAPACITY 4
#define MAX_LINE_LENGTH 1024
#define USAGE_ERROR "Usage: ./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>"
#define INPUT_FILES_ERROR "Error in input files."
#define OUTPUT_FILE_ERROR "Error in output file."
// ------------------------------ structures ----------------------------
/**
 * @struct Person - stores an individual personal details
 */
typedef struct Person
{
    unsigned long id;
    char* name;
    float age;
    float risk; // infection probability
} Person;

/**
 * @struct InfectionChain - represents a chain of infection by storing a list of people ("contacts")
 * and their total number ("size" of the chain).
 */
typedef struct InfectionChain
{
    Person* contacts;
    int size; // number of contacts (including the infector)
} InfectionChain;
// ------------------------------ functions -----------------------------
/**
 * De-allocates dynamic memory of a list of persons
 */
void freeContacts(Person* contacts, int size)
{
    if (contacts != NULL)
    {
        for (int i = 0; i < size; ++i)
        {
            free(contacts[i].name);
            contacts[i].name = NULL;
        }
        free(contacts);
    }
}

/**
 * De-allocates dynamic memory of a given chain
 */
 void freeChain(InfectionChain* chain)
{
     if (chain != NULL)
     {
         freeContacts(chain->contacts, chain->size);
         free(chain);
     }
}

/**
 * Prints an error message and exit the program
 * @param errorMsg - error message to display
 */
void printErrorAndExit(const char* errorMsg)
{
    fprintf(stderr, "%s\n", errorMsg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Compares between two persons according to ID
 */
int compById(const void *p1, const void *p2)
{
    unsigned long id1 = (*(Person*)p1).id;
    unsigned long id2 = (*(Person*)p2).id;
    return (id1 < id2) ? -1 : (id1 > id2);
}

/**
 * @brief Compares between two persons according to risk level
 */
int compByRisk(const void *p1, const void *p2)
{
    float r1 = (*(Person*)p1).risk;
    float r2 = (*(Person*)p2).risk;
    return (r1 < r2) ? -1 : (r1 > r2);
}

/**
 * Write output to a file - medical instruction for each person, ordered by severity
 * @param chain - people list
 */
void writeResults(InfectionChain* chain)
{
    FILE *fpOut = fopen(OUTPUT_FILE, "w");
    for (int i = chain->size - 1; i >= 0; i--)
    {
        if (chain->contacts[i].risk >= MEDICAL_SUPERVISION_THRESHOLD)
        {
            fprintf(fpOut, MEDICAL_SUPERVISION_THRESHOLD_MSG, chain->contacts[i].name, chain->contacts[i].id);
        }
        else if (chain->contacts[i].risk >= REGULAR_QUARANTINE_THRESHOLD)
        {
            fprintf(fpOut, REGULAR_QUARANTINE_MSG, chain->contacts[i].name, chain->contacts[i].id);
        }
        else
        {
            fprintf(fpOut, CLEAN_MSG, chain->contacts[i].name, chain->contacts[i].id);
        }
    }
    if (ferror(fpOut))
    {
        fclose(fpOut);
        freeChain(chain);
        printErrorAndExit(OUTPUT_FILE_ERROR);
    }
    fclose(fpOut);
}

/**
 * @brief Calculates the probability that one person has infected another person in a meeting
 * @param dist - distance between two people
 * @param time - for how long two people were closed to each other
 * @return infection probability
 */
float crna(const float dist, const float time)
{
    return (time * MIN_DISTANCE) / (dist * MAX_TIME);
}

/**
 * Calculates the infection probability of a contact
 * @param chain - people list
 * @param spreaderId - the potential infector id
 * @param contactId - the contact (who came across the spreader) id
 * @param dist - distance between the spreader the contact
 * @param time - meeting duration
 * @return current spreader
 */
Person* setContactRisk(InfectionChain* chain, const unsigned long spreaderId, const unsigned long contactId,
                       const float dist, const float time, Person* currSpreader)
{
    Person *currContact, key;

    if (currSpreader == NULL)  // the first person is always the infector!
    {
        key.id = spreaderId;
        Person* infector = bsearch(&key, chain->contacts, chain->size, sizeof(Person), compById);
        infector->risk = SICK;
        return infector;
    }

    // Search for the current spreader in the chain if necessary
    if (spreaderId != currSpreader->id)
    {
        key.id = spreaderId;  // value to search for
        currSpreader = bsearch(&key, chain->contacts, chain->size, sizeof(Person), compById);
    }

    // Search for the current contact in the chain
    key.id = contactId;
    currContact = bsearch(&key, chain->contacts, chain->size, sizeof(Person), compById);

    // Update contact risk
    currContact->risk = currSpreader->risk * crna(dist, time);
    return currSpreader;
}

/**
 * Read a single meeting information and updates the contact's risk of infection
 * @param chain - people list
 * @param buffer - string of the meeting details
 * @param d - delimiter for splitting buffer string
 * @param prevSpreader - the spreader from previous line
 * @return the current spreader
 */
Person* readMeetingLine(InfectionChain *chain, char* buffer, const char* d, Person* prevSpreader)
{
    strtok(buffer, "\n");

    // Parse current spreader ID
    char* arg = strtok(buffer, d); // stores each detail at a time
    unsigned long spreaderId = (int)strtol(arg, NULL, 10);

    if (!prevSpreader)
    {
        return setContactRisk(chain, spreaderId, 0, 0, 0, NULL);
    }

    // Parse meeting details
    arg = strtok(NULL, d);
    unsigned long contactId = (int)strtol(arg, NULL, 10);
    arg = strtok(NULL, d);
    float dist = (float)strtod(arg, NULL);
    arg = strtok(NULL, d);
    float time = (float)strtod(arg, NULL);

    // Update the infection probability of the contact
    return setContactRisk(chain, spreaderId, contactId, dist, time, prevSpreader);
}

/**
 * Calculates the infection probability ('risk' field) of each person, by reading meeting details from a given file
 * @param chain - people list
 * @param filePath - path to <meeting.in> file
 */
void calculateRisks(InfectionChain* chain, const char* filePath)
{
    FILE *fpIn = fopen(filePath, "r");

    // Check file existence
    if (!fpIn)
    {
        freeChain(chain);
        printErrorAndExit(INPUT_FILES_ERROR);
    }

    // Initialize variables
    Person* prevSpreader = NULL;
    const char dlm[2] = " "; // Text delimiter
    char buffer[MAX_LINE_LENGTH];

    // Read file
    while (fgets(buffer, sizeof(buffer), fpIn) != NULL)
    {
        prevSpreader = readMeetingLine(chain, buffer, dlm, prevSpreader);
    }

    if (ferror(fpIn))
    {
        freeChain(chain);
        printErrorAndExit(INPUT_FILES_ERROR);
    }
    fclose(fpIn);
}

/**
 * Expands an array of persons
 * @param arrPtr - pointer to an array
 * @param capacity - current array size
 * @return new array size
 */
int resizeArray(Person** personsPtr, const int capacity)
{
    int newCapacity = capacity * ARR_SIZE_MULTIPLIER;
    Person* p = (Person*)realloc(*personsPtr, newCapacity * sizeof(Person));
    if (p == NULL)
    {
        return FAILURE;
    }
    *personsPtr = p;
    return newCapacity;
}

/**
 * Add an individual to the people list
 * @param p - pointer to people list
 * @param buffer - string of personal details
 * @param d - delimiter for splitting buffer string
 * @param idx - index of the first empty slot in the people list
 */
void addContact(Person** p, char* buffer, const char* d, const int idx)
{
    Person* persons = *p;
    char* arg;
    strtok(buffer, "\n");

    // Parse person details (name, id, age)
    arg = strtok(buffer, d);
    char* name = (char*)malloc(strlen(arg) + 1);
    if (!name)
    {
        freeContacts(*p, idx);
        printErrorAndExit(STANDARD_LIB_ERR_MSG);
    }

    strcpy(name, arg);
    persons[idx].name = name;

    arg = strtok(NULL, d);
    persons[idx].id = (int)strtol(arg, NULL, 10);

    arg = strtok(NULL, d);
    persons[idx].age = (float)strtod(arg, NULL);

    persons[idx].risk = DEFAULT_RISK;
}

/**
 * Load contacts information into a list
 * @param contacts - pointer to contacts list
 * @param capacity - contacts list size
 * @param filePath - path to input file
 * @return total number of contacts
 */
int buildContactsList(Person** contacts, int capacity, const char* filePath)
{
    FILE *fpIn = fopen(filePath, "r");

    // Check file existence
    if (!fpIn)
    {
        freeContacts(*contacts, capacity);
        printErrorAndExit(INPUT_FILES_ERROR);
    }

    // Initialize variables
    int numOfContacts = 0; // Line counter
    const char dlm[2] = " "; // Text delimiter
    char buffer[MAX_LINE_LENGTH];

    // Read file
    while (fgets(buffer, sizeof(buffer), fpIn) != NULL)
    {
        addContact(contacts, buffer, dlm, numOfContacts);
        numOfContacts++;

        // Check array overflow and resize if necessary
        if (contacts != NULL && numOfContacts >= capacity)
        {
            int newCapacity = resizeArray(contacts, capacity);
            if (newCapacity == FAILURE)
            {
                fclose(fpIn);
                freeContacts(*contacts, capacity);
                printErrorAndExit(STANDARD_LIB_ERR_MSG);
            }
            capacity = newCapacity;
        }
    }

    if (ferror(fpIn))
    {
        freeContacts(*contacts, capacity);
        printErrorAndExit(INPUT_FILES_ERROR);
    }
    fclose(fpIn);
    return numOfContacts;
}

/**
 * Stores people information given from a file inside a list
 * @param filePath - path to <people.in> file
 * @return list of people and their number
 */
InfectionChain* buildChain(const char* filePath)
{
    Person *contacts = (Person*)malloc(INITIAL_ARR_CAPACITY * sizeof(Person));
    if (contacts == NULL)
    {
        printErrorAndExit(STANDARD_LIB_ERR_MSG);
    }

    // Build list of contacts
    int numOfContacts = buildContactsList(&contacts, INITIAL_ARR_CAPACITY, filePath);
    InfectionChain *chain = (InfectionChain*)malloc(sizeof(InfectionChain));
    if (chain == NULL)
    {
        freeContacts(contacts, numOfContacts);
        printErrorAndExit(STANDARD_LIB_ERR_MSG);
    }

    chain->size = numOfContacts;
    chain->contacts = contacts;
    return chain;
}

/**
 * @brief Main function - controls program flow
 */
int main(int argc, char *argv[])
{
    // Validate command-line arguments
    if (argc != NUM_OF_CMD_ARGS)
    {
        printErrorAndExit(USAGE_ERROR);
    }

    // Load people data
    InfectionChain *chain = buildChain((argv[1]));

    // Sort people list by ID
    qsort(chain->contacts, chain->size, sizeof(Person), compById);

    // Load meeting data & calculate infection probability for each person
    calculateRisks(chain, argv[2]);

    // Sort people list by severity
    qsort(chain->contacts, chain->size, sizeof(Person), compByRisk);

    // Write results and say goodbye :)
    writeResults(chain);
    freeChain(chain);

    return EXIT_SUCCESS;
}