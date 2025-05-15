/*gcc main.c -o main 
echo "(1+2+(3+4))+5" | ./main   */

#include <stdio.h>
 #include <string.h>

 enum Token {
     TOK_NUM, TOK_PLUS, TOK_LPAREN, TOK_RPAREN, TOK_EOF, TOK_INVALID
 };

 void exit(int); 
 
 static char  input[1024];   
 static int   pos  = 0;      
 static char  lexeme[64];    
 static enum Token token;   
 
 static inline char peek(void)          { return input[pos]; }
 static inline char getch(void)         { return input[pos] ? input[pos++] : '\0'; }
 
 static void next_token(void)
 {
     while (peek() == ' ' || peek() == '\t' || peek() == '\n') getch();   
 
     char c = peek();
     if (c == '\0') { token = TOK_EOF; return; }
 
     if (c >= '0' && c <= '9') {           
         int len = 0;
         while (peek() >= '0' && peek() <= '9' && len < (int)sizeof(lexeme) - 1)
             lexeme[len++] = getch();
         lexeme[len] = '\0';
         token = TOK_NUM;
         return;
     }
 
     switch (c) {
         case '+': token = TOK_PLUS;   getch(); return;
         case '(': token = TOK_LPAREN; getch(); return;
         case ')': token = TOK_RPAREN; getch(); return;
         default : token = TOK_INVALID; getch(); return;
     }
 }
 
 
 static void parse_error(const char *msg)
 {
     fprintf(stderr, "Parse error: %s  (pos=%d, ch='%c')\n", msg, pos, input[pos]);
     exit(1);
 }
 
 static void show(const char *rule, int indent)
 {
     for (int i = 0; i < indent * 2; ++i) putchar(' ');
     puts(rule);
 }

 static void parse_S(int);    
 static void parse_Sp(int);
 static void parse_E(int);
 
 static void parse_S(int ind)
 {
     show("S -> E S'", ind);
     parse_E(ind + 1);
     parse_Sp(ind + 1);
 }
 
 static void parse_Sp(int ind)
 {
     if (token == TOK_PLUS) {
         show("S' -> + S", ind);
         next_token();         
         parse_S(ind + 1);
     } else {
         if (token == TOK_RPAREN || token == TOK_EOF) {
             show("S' -> eps", ind);
             return;
         }
         parse_error("expecting + | ) | EOF");
     }
 }
 
 static void parse_E(int ind)
 {
    if (token == TOK_NUM) {
        char buf[128];
        sprintf(buf, "E -> num");
        show(buf, ind);

        sprintf(buf, "num => %s", lexeme);
        show(buf, ind + 1);

        next_token();
        return;
    }
     if (token == TOK_LPAREN) {
         show("E -> ( S )", ind);
         next_token();         
         parse_S(ind + 1);
         if (token != TOK_RPAREN) parse_error("missing )");
         next_token();       
         return;
     }
     parse_error("expecting number or (");
 }
 
 int main(void)
 {
     if (!fgets(input, sizeof(input), stdin)) return 0;
 
     pos = 0;
     next_token();      
     parse_S(0);        
     if (token != TOK_EOF) parse_error("extra tokens after valid input");
 
     return 0;
 }
 