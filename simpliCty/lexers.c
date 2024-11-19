#include "lexers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t line_number = 1;        // Global line number


// Helper function to create a token
Token *create_token(TokenType type, const char *value, size_t line_num) {
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value);
    token->line_num = line_num;
    return token;
}


const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_ARITHMETIC_OPERATOR: return "ARITHMETIC_OPERATOR";
        case TOKEN_RELATIONAL_OPERATOR: return "RELATIONAL_OPERATOR";
        case TOKEN_LOGICAL_OPERATOR: return "LOGICAL_OPERATOR";
        case TOKEN_ASSIGNMENT_OPERATOR: return "ASSIGNMENT_OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_UNARY_OPERATOR: return "UNARY_OPERATOR";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_EOF: return "EOF";
        default: return "UNDEFINED";
    }
}

void print_token(const Token *token) {
    printf("TOKEN: %s | TYPE: %s | LINE: %zu\n", 
           token->value, 
           token_type_to_string(token->type), 
           token->line_num);
}



// Free token memory
void free_token(Token *token) {
    if (token->value) free(token->value);
    free(token);
}

Token *generate_number(const char *source, int *index) {
    char buffer[32] = {0};
    int buffer_index = 0;

    while (isdigit(source[*index])) {
        buffer[buffer_index++] = source[(*index)++];
    }
    return create_token(TOKEN_NUMBER, buffer, line_number);
}


// Classify keywords or identifiers using switch-case
Token *classify_keyword_or_identifier(const char *lexeme) {
    int len = strlen(lexeme);

    // Start at the "initial state" (q0)
    switch (lexeme[0]) {
        case 'b': // Check for "break"
            if (len == 5) { // Ensure the word has the correct length
                switch (lexeme[1]) {
                    case 'r':
                        switch (lexeme[2]) {
                            case 'e':
                                switch (lexeme[3]) {
                                    case 'a':
                                        switch (lexeme[4]) {
                                            case 'k':
                                                return create_token(TOKEN_KEYWORD, "BREAK", line_number);
                                        }
                                        break;
                                }
                                break;
                        }
                        break;
                }
            }
            break;

        case 'i': // Check for "int" or "if"
            if (len == 3 && lexeme[1] == 'n' && lexeme[2] == 't') {
                return create_token(TOKEN_KEYWORD, "INT", line_number);
            } else if (len == 2 && lexeme[1] == 'f') {
                return create_token(TOKEN_KEYWORD, "IF", line_number);
            }
            break;

        case 'r': // Check for "return"
            if (len == 6) {
                switch (lexeme[1]) {
                    case 'e':
                        switch (lexeme[2]) {
                            case 't':
                                switch (lexeme[3]) {
                                    case 'u':
                                        switch (lexeme[4]) {
                                            case 'r':
                                                switch (lexeme[5]) {
                                                    case 'n':
                                                        return create_token(TOKEN_KEYWORD, "RETURN", line_number);
                                                }
                                                break;
                                        }
                                        break;
                                }
                                break;
                        }
                        break;
                }
            }
            break;

        case 'w': // Check for "while"
            if (len == 5) {
                switch (lexeme[1]) {
                    case 'h':
                        switch (lexeme[2]) {
                            case 'i':
                                switch (lexeme[3]) {
                                    case 'l':
                                        switch (lexeme[4]) {
                                            case 'e':
                                                return create_token(TOKEN_KEYWORD, "WHILE", line_number);
                                        }
                                        break;
                                }
                                break;
                        }
                        break;
                }
            }
            break;

        default:
            break;
    }

    // If no keyword is matched, classify as an identifier
    return create_token(TOKEN_IDENTIFIER, lexeme, line_number);
}


// Classifier for operators
Token *classify_operator(const char *source, int *index) {
    char op = source[*index];
    (*index)++;

    // Handle multi-character operators
    if (op == '=' && source[*index] == '=') {
        (*index)++;
        return create_token(TOKEN_RELATIONAL_OPERATOR, "==", line_number);
    } else if (op == '<' && source[*index] == '=') {
        (*index)++;
        return create_token(TOKEN_RELATIONAL_OPERATOR, "<=", line_number);
    } else if (op == '>' && source[*index] == '=') {
        (*index)++;
        return create_token(TOKEN_RELATIONAL_OPERATOR, ">=", line_number);
    } else if (op == '!' && source[*index] == '=') {
        (*index)++;
        return create_token(TOKEN_RELATIONAL_OPERATOR, "!=", line_number);
    } else if (op == '&' && source[*index] == '&') {
        (*index)++;
        return create_token(TOKEN_LOGICAL_OPERATOR, "&&", line_number);
    } else if (op == '|' && source[*index] == '|') {
        (*index)++;
        return create_token(TOKEN_LOGICAL_OPERATOR, "||", line_number);
    }

    // Single-character operators
    switch (op) {
        case '+': return create_token(TOKEN_ARITHMETIC_OPERATOR, "+", line_number);
        case '-': return create_token(TOKEN_ARITHMETIC_OPERATOR, "-", line_number);
        case '*': return create_token(TOKEN_ARITHMETIC_OPERATOR, "*", line_number);
        case '/': return create_token(TOKEN_ARITHMETIC_OPERATOR, "/", line_number);
        case '=': return create_token(TOKEN_ASSIGNMENT_OPERATOR, "=", line_number);
        case '!': return create_token(TOKEN_UNARY_OPERATOR, "!", line_number);
        case '<': return create_token(TOKEN_RELATIONAL_OPERATOR, "<", line_number);
        case '>': return create_token(TOKEN_RELATIONAL_OPERATOR, ">", line_number);
    }

    // Default: Unknown operator
    char unknown_op[2] = {op, '\0'};
    return create_token(TOKEN_UNKNOWN, unknown_op, line_number);
}

// Classifier for delimiters
Token *classify_delimiter(char c, size_t line_number) {
    switch (c) {
        case ';': return create_token(TOKEN_DELIMITER, ";", line_number);
        case ',': return create_token(TOKEN_DELIMITER, ",", line_number);
        case '(': return create_token(TOKEN_DELIMITER, "(", line_number);
        case ')': return create_token(TOKEN_DELIMITER, ")", line_number);
        case '{': return create_token(TOKEN_DELIMITER, "{", line_number);
        case '}': return create_token(TOKEN_DELIMITER, "}", line_number);
        case '[': return create_token(TOKEN_DELIMITER, "[", line_number);
        case ']': return create_token(TOKEN_DELIMITER, "]", line_number);
        default: {
            char unknown[2] = {c, '\0'};
            return create_token(TOKEN_UNKNOWN, unknown, line_number);
        }
    }
}


// Tokenize source code into an array of tokens
Token **tokenize(const char *source, size_t *token_count) {
    size_t capacity = 10;
    Token **tokens = malloc(capacity * sizeof(Token *));
    *token_count = 0;
    int index = 0;
    int length = strlen(source);

    while (index < length) {
        char c = source[index];

        // Skip white spaces and track line numbers
        if (isspace(c)) {
            if (c == '\n') line_number++;
            index++;
            continue;
        }

        Token *token = NULL;

        // Numbers
        if (isdigit(c)) {
            token = generate_number(source, &index);
        }
        // Keywords or Identifiers
        else if (isalpha(c) || c == '_') {
            char buffer[64] = {0};
            int buffer_index = 0;

            while (isalnum(source[index]) || source[index] == '_') {
                buffer[buffer_index++] = source[index++];
            }

            token = classify_keyword_or_identifier(buffer); // Call the improved function
        }
        // Operators
        else if (strchr("+-*/=<>!&|", c)) {
            token = classify_operator(source, &index);
        }
        // Delimiters
        else if (strchr(";{},()[]", c)) {
            token = classify_delimiter(c, line_number);
            index++;  // Advance after handling delimiter
        }
        // Handle unrecognized characters
        else {
            fprintf(stderr, "Unrecognized character '%c' at line %zu\n", c, line_number);
            index++;
            continue;
        }

        // Store token
        if (token) {
            if (*token_count == capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token *));
            }
            tokens[(*token_count)++] = token;
        }
    }

    // Add END_OF_TOKENS marker
    tokens[*token_count] = create_token(TOKEN_EOF, "EOF", line_number);
    return tokens;
}


// Lexer function to read file and tokenize content
Token **lexer(FILE *file, size_t *token_count) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    Token **tokens = tokenize(buffer, token_count);
    free(buffer);

    return tokens;
}