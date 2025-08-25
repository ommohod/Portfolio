#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

#define MAX_PROJECTS 10
#define MAX_TEXT_LENGTH 256
#define FILENAME "portfolio_data.txt"

// Structure to hold portfolio data
typedef struct {
    char about[MAX_TEXT_LENGTH];
    char projects[MAX_PROJECTS][MAX_TEXT_LENGTH];
    int project_count;
    char phone[MAX_TEXT_LENGTH];
    char linkedin[MAX_TEXT_LENGTH];
    char gmail[MAX_TEXT_LENGTH];
    char contact[MAX_TEXT_LENGTH];
} Portfolio;

Portfolio portfolio;

bool contact_initialized = false; // Flag to check if contact details have been initialized

// Function to initialize portfolio data
void init_portfolio() {
    strcpy(portfolio.about, "No information added.");
    portfolio.project_count = 0;
    strcpy(portfolio.phone, "No phone number added.");
    strcpy(portfolio.linkedin, "No LinkedIn profile added.");
    strcpy(portfolio.gmail, "No Gmail address added.");
}   

// Function to load portfolio from a file
void load_portfolio() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No saved portfolio found. Starting a new portfolio.\n");
        return;
    }
    fread(&portfolio, sizeof(Portfolio), 1, file);
    fclose(file);
    printf("Portfolio loaded successfully.\n");
}

// Function to save portfolio to a file
void save_portfolio() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        // Use perror for detailed error message
        perror("Error opening file");
        return;
    }
    size_t written = fwrite(&portfolio, sizeof(Portfolio), 1, file);
    if (written != 1) {
        printf("Error writing to file.\n");
    } else {
        printf("Portfolio saved successfully.\n");
    }
    fclose(file);
}



// Function to display the main menu
void show_menu() {
    printf("\n==============================\n");
    printf(" Portfolio Creation CLI\n");
    printf("==============================\n");
    printf("Commands:\n");
    printf("  edit_about     - Edit About Me section\n");
    printf("  add_project    - Add a new project\n");
    printf("  view_projects  - View all projects\n");
    printf("  edit_project   - Edit a specific project\n");
    printf("  delete_project - Delete a specific project\n");
    printf("  edit_contact   - Edit contact information\n");
    printf("  display        - Display entire portfolio\n");
    printf("  save           - Save portfolio to file\n");
    printf("  load           - Load portfolio from file\n");
    printf("  exit           - Exit the CLI\n");
    printf("Enter a command: ");
}

bool about_initialized = false; // Flag to check if initial details have been added

// Function to edit the About Me section
void edit_about() {
    printf("\n--- Edit About Me ---\n");

    if (!about_initialized || strcmp(portfolio.about, "No information added.") == 0) {
        // First time setup
        char name[MAX_TEXT_LENGTH];
        char skills[MAX_TEXT_LENGTH];
        char experience[MAX_TEXT_LENGTH];
        char education[MAX_TEXT_LENGTH];
        char achievements[MAX_TEXT_LENGTH] = ""; // Achievements can be left empty

        printf("Enter your name: ");
        fgets(name, MAX_TEXT_LENGTH, stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove newline

        printf("Enter your skills: ");
        fgets(skills, MAX_TEXT_LENGTH, stdin);
        skills[strcspn(skills, "\n")] = '\0';

        printf("Enter your experience: ");
        fgets(experience, MAX_TEXT_LENGTH, stdin);
        experience[strcspn(experience, "\n")] = '\0';

        printf("Enter your education: ");
        fgets(education, MAX_TEXT_LENGTH, stdin);
        education[strcspn(education, "\n")] = '\0';

        printf("Enter your achievements (optional, press Enter to skip): ");
        fgets(achievements, MAX_TEXT_LENGTH, stdin);
        achievements[strcspn(achievements, "\n")] = '\0';

        // Format and store in portfolio.about
        snprintf(portfolio.about, MAX_TEXT_LENGTH,
                 "Name: %s\nSkills: %s\nExperience: %s\nEducation: %s\nAchievements: %s",
                 name, skills, experience, education, achievements[0] ? achievements : "None");

        about_initialized = true; // Mark as initialized
        printf("About Me section initialized successfully.\n");

    } else {
        // Append new information to the existing sections
        printf("Existing About Me:\n%s\n\n", portfolio.about);

        char additional_info[MAX_TEXT_LENGTH];
        printf("Enter additional information to append to the 'About Me' section: ");
        fgets(additional_info, MAX_TEXT_LENGTH, stdin);
        additional_info[strcspn(additional_info, "\n")] = '\0'; // Remove newline

        // Ensure there's enough space before appending
        if (strlen(portfolio.about) + strlen(additional_info) + 2 < MAX_TEXT_LENGTH) {
            // Append new info with a separator
            strncat(portfolio.about, "\n", MAX_TEXT_LENGTH - strlen(portfolio.about) - 1);
            strncat(portfolio.about, additional_info, MAX_TEXT_LENGTH - strlen(portfolio.about) - 1);
            printf("Information appended to the About Me section.\n");
        } else {
            printf("Error: Not enough space to add additional information.\n");
        }
    }
}
 
// Function to add a project
void add_project() {
    if (portfolio.project_count < MAX_PROJECTS) {
        printf("\n--- Add a Project ---\n");
        printf("Enter the project description: ");
        fgets(portfolio.projects[portfolio.project_count], MAX_TEXT_LENGTH, stdin);
        portfolio.projects[portfolio.project_count][strcspn(portfolio.projects[portfolio.project_count], "\n")] = '\0';
        portfolio.project_count++;
        printf("Project added successfully.\n");
    } else {
        printf("Maximum number of projects reached.\n");
    }
}


// Function to view all projects
void view_projects() {
    printf("\n--- Projects ---\n");
    for (int i = 0; i < portfolio.project_count; i++) {
        printf("  %d. %s\n", i + 1, portfolio.projects[i]);
    }
}

// Function to edit a specific project
void edit_project() {
    int index;
    char new_description[MAX_TEXT_LENGTH];
    char choice;

    // Show the list of projects
    view_projects();
    printf("Enter the project number to edit: ");
    scanf("%d", &index);
    getchar(); // Clear the newline character from input buffer

    if (index > 0 && index <= portfolio.project_count) {
        // Prompt user for overwrite or append
        printf("Would you like to (O)verwrite or (A)ppend to this project? (O/A): ");
        choice = getchar();
        getchar(); // Clear the newline character from input buffer

        printf("Enter new description for project %d: ", index);
        fgets(new_description, MAX_TEXT_LENGTH, stdin);
        new_description[strcspn(new_description, "\n")] = '\0'; // Remove trailing newline

        // Handle user's choice for overwrite or append
        if (choice == 'O' || choice == 'o') {
            // Overwrite the project description
            strncpy(portfolio.projects[index - 1], new_description, MAX_TEXT_LENGTH - 1);
            portfolio.projects[index - 1][MAX_TEXT_LENGTH - 1] = '\0'; // Ensure null-termination
            printf("Project %d description overwritten successfully.\n", index);
        } else if (choice == 'A' || choice == 'a') {
            // Append to the project description
            int available_space = MAX_TEXT_LENGTH - strlen(portfolio.projects[index - 1]) - 1;

            if (strlen(new_description) > available_space) {
                printf("Error: Not enough space to add additional information.\n");
            } else {
                // Add a space before appending if there's existing content
                if (strlen(portfolio.projects[index - 1]) > 0) {
                    strncat(portfolio.projects[index - 1], " ", available_space);
                }
                // Append the new input
                strncat(portfolio.projects[index - 1], new_description, available_space);
                printf("Project %d description appended successfully.\n", index);
            }
        } else {
            printf("Invalid choice. Please enter 'O' to overwrite or 'A' to append.\n");
        }
    } else {
        printf("Invalid project number.\n");
    }
}


// Function to delete a specific project
void delete_project() {
    int index;
    if (portfolio.project_count == 0) {
        printf("No projects to delete.\n");
        return;
    }

    view_projects();
    printf("Enter the project number to delete: ");
    if (scanf("%d", &index) != 1) { // Ensure a valid integer is entered
        printf("Invalid input. Please enter a valid project number.\n");
        while (getchar() != '\n'); // Clear the input buffer
        return;
    }

    getchar(); // Clear the newline character left by scanf

    if (index > 0 && index <= portfolio.project_count) {
        for (int i = index - 1; i < portfolio.project_count - 1; i++) {
            strcpy(portfolio.projects[i], portfolio.projects[i + 1]);
        }
        portfolio.project_count--;
        printf("Project deleted successfully.\n");
    } else {
        printf("Invalid project number.\n");
    }
}


// Function to check if a string represents a valid phone number (only digits and 10 digits long)
bool is_valid_phone_number(const char* phone) {
    if (strlen(phone) != 10) return false; // Check if the phone number is exactly 10 digits long

    // Check if all characters are digits
    for (int i = 0; i < 10; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return false;
        }
    }
    return true;
}

// Function to edit contact information
void edit_contact() {
    printf("\n--- Edit Contact Information ---\n");

    if (!contact_initialized) {
        // First-time setup for contact details
        char phone[MAX_TEXT_LENGTH];
        char gmail[MAX_TEXT_LENGTH];
        char linkedin[MAX_TEXT_LENGTH];

        // Phone number input with validation
        while (1) {
            printf("Enter your phone number (10 digits only): ");
            fgets(phone, MAX_TEXT_LENGTH, stdin);
            phone[strcspn(phone, "\n")] = '\0'; // Remove newline character

            if (is_valid_phone_number(phone)) {
                strcpy(portfolio.phone, phone);
                break;
            } else {
                printf("Error: Please enter a valid phone number (exactly 10 digits).\n");
            }
        }

        printf("Enter your Gmail: ");
        fgets(gmail, MAX_TEXT_LENGTH, stdin);
        gmail[strcspn(gmail, "\n")] = '\0';

        printf("Enter your LinkedIn profile URL: ");
        fgets(linkedin, MAX_TEXT_LENGTH, stdin);
        linkedin[strcspn(linkedin, "\n")] = '\0';

        // Store the information into separate fields
        strcpy(portfolio.gmail, gmail);
        strcpy(portfolio.linkedin, linkedin);

        contact_initialized = true; // Mark as initialized
        printf("Contact information initialized successfully.\n");

    } else {
        // Display current contact details
        printf("Existing Contact Information:\n");
        printf("Phone: %s\n", portfolio.phone);
        printf("Gmail: %s\n", portfolio.gmail);
        printf("LinkedIn: %s\n\n", portfolio.linkedin);

        char choice;
        printf("Do you want to update any of the contact information? (Y/N): ");
        scanf(" %c", &choice); // Notice the space before %c to clear any newline characters
        getchar(); // Clear any extra newline character from the input buffer

        if (choice == 'Y' || choice == 'y') {
            char phone[MAX_TEXT_LENGTH], gmail[MAX_TEXT_LENGTH], linkedin[MAX_TEXT_LENGTH];
            
            // Phone number input with validation
            printf("Enter new phone number (leave blank to keep existing): ");
            fgets(phone, MAX_TEXT_LENGTH, stdin);
            phone[strcspn(phone, "\n")] = '\0'; // Remove newline character
            if (strlen(phone) > 0) {
                if (is_valid_phone_number(phone)) {
                    strcpy(portfolio.phone, phone);
                } else {
                    printf("Error: Please enter a valid phone number (exactly 10 digits).\n");
                    return; // Return immediately if phone number is invalid
                }
            }

            printf("Enter new Gmail (leave blank to keep existing): ");
            fgets(gmail, MAX_TEXT_LENGTH, stdin);
            gmail[strcspn(gmail, "\n")] = '\0';
            if (strlen(gmail) > 0) {
                strcpy(portfolio.gmail, gmail);
            }

            printf("Enter new LinkedIn profile (leave blank to keep existing): ");
            fgets(linkedin, MAX_TEXT_LENGTH, stdin);
            linkedin[strcspn(linkedin, "\n")] = '\0';
            if (strlen(linkedin) > 0) {
                strcpy(portfolio.linkedin, linkedin);
            }

            printf("Contact information updated successfully.\n");
        }
    }
}


// Function to display the portfolio
void display_portfolio() {
    printf("\n--- Portfolio ---\n");
    printf("About Me: %s\n", portfolio.about);
    printf("Projects:\n");
    if (portfolio.project_count == 0) {
        printf("  No projects added.\n");
    } else {
        for (int i = 0; i < portfolio.project_count; i++) {
            printf("  %d. %s\n", i + 1, portfolio.projects[i]);
        }
    }
    printf("Contact Information:\n");
    printf("  Phone: %s\n", portfolio.phone);
    printf("  Gmail: %s\n", portfolio.gmail);
    printf("  LinkedIn: %s\n\n", portfolio.linkedin);
}



int main() {
    char command[20];
    init_portfolio();
    load_portfolio();
    while (1) {
        show_menu();
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "edit_about") == 0) {
            edit_about();
        } else if (strcmp(command, "add_project") == 0) {
            add_project();
        } else if (strcmp(command, "view_projects") == 0) {
            view_projects();
        } else if (strcmp(command, "edit_project") == 0) {
            edit_project();
        } else if (strcmp(command, "delete_project") == 0) {
            delete_project();
        } else if (strcmp(command, "edit_contact") == 0) {
            edit_contact();
        } else if (strcmp(command, "display") == 0) {
            display_portfolio();
        } else if (strcmp(command, "save") == 0) {
            save_portfolio();
        } else if (strcmp(command, "load") == 0) {
            load_portfolio();
        } else if (strcmp(command, "exit") == 0) {
            save_portfolio();
            printf("Exiting the portfolio CLI. Goodbye!\n");
            break;
        } else {
            printf("Unknown command. Please enter a valid command.\n");
        }
}
    return 0;
}