#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SHOWEXP 1
#define SHOWPI 0

int main() {
    puts("Assuming you are solving for Pr[B|A]:\n");
    
    double pos_prob, neg_prob;
    fputs("What is Pr[B]? (e.g. %of ppl who are bad sellers, etc.) ", stdout);
    scanf("%lg", &pos_prob);
    neg_prob = 1.0 - pos_prob;

    unsigned int num_events;
    printf("How many different events are occur? (e.g. pos and neg rating = 2) ");
    scanf("%u", &num_events);

    double ans[2] = {pos_prob, neg_prob};
    double cond_pos_prob, cond_neg_prob;
    unsigned int event_count;


    // Setup string building
    char **ans_str = malloc(2*sizeof(char *));
    asprintf(&ans_str[0], "%g", pos_prob);
    asprintf(&ans_str[1], "%g", neg_prob);
    char **ans_str_tmp = malloc(2*sizeof(char *));

    char **latex_pi_str, **latex_pi_str_tmp;
    if (SHOWPI) {
        latex_pi_str = malloc(2*sizeof(char *));
        asprintf(&latex_pi_str[0], "%g", pos_prob);
        asprintf(&latex_pi_str[1], "%g", neg_prob);
     
        latex_pi_str_tmp = malloc(2*sizeof(char *));
    }

    char **latex_exp_str, **latex_exp_str_tmp;
    if (SHOWEXP) {
        latex_exp_str = malloc(2*sizeof(char *));
        asprintf(&latex_exp_str[0], "%g", pos_prob);
        asprintf(&latex_exp_str[1], "%g", neg_prob);

        latex_exp_str_tmp = malloc(2*sizeof(char *));
    }


    int i;
    for (i = 1; i <= num_events; ++i) {
        printf("What is Pr[A_%d|B]? (e.g. prob you "
               "are a bad seller with pos rating) ", i);
        scanf("%lg", &cond_pos_prob);

        printf("What is Pr[A_%d|!B]? (e.g. prob you "
               "are not a bad seller with pos rating) ", i);
        scanf("%lg", &cond_neg_prob);

        printf("How many times did this event occur? ");
        scanf("%u", &event_count);


        ans[0] *= pow(cond_pos_prob, (double)event_count);
        ans[1] *= pow(cond_neg_prob, (double)event_count);


        asprintf(&ans_str_tmp[0], "%s x %g^%d",
                 ans_str[0], cond_pos_prob, event_count);
        asprintf(&ans_str_tmp[1], "%s x %g^%d",
                 ans_str[1], cond_neg_prob, event_count);

        free(ans_str[0]);
        free(ans_str[1]);

        ans_str[0] = ans_str_tmp[0];
        ans_str[1] = ans_str_tmp[1];


        if (SHOWPI) {
            asprintf(&latex_pi_str_tmp[0],
                     "%s \\cdot \\left({\\prod_{i=1}^{%u} %g}\\right)",
                     latex_pi_str[0], event_count, cond_pos_prob);
            asprintf(&latex_pi_str_tmp[1],
                     "%s \\cdot \\left({\\prod_{i=1}^{%u} %g}\\right)",
                     latex_pi_str[1], event_count, cond_neg_prob);

            free(latex_pi_str[0]);
            free(latex_pi_str[1]);

            latex_pi_str[0] = latex_pi_str_tmp[0];
            latex_pi_str[1] = latex_pi_str_tmp[1];
        }


        if (SHOWEXP) {
            asprintf(&latex_exp_str_tmp[0], "%s \\times %g^{%u}",
                                            latex_exp_str[0],
                                            cond_pos_prob, event_count);
            asprintf(&latex_exp_str_tmp[1], "%s \\times %g^{%u}",
                                            latex_exp_str[1],
                                            cond_neg_prob, event_count);
        
            // Effectively cutoff the exponent portion
            if (event_count == 1) {
                latex_exp_str_tmp[0][strlen(latex_exp_str_tmp[0])-4] = '\0';
                latex_exp_str_tmp[1][strlen(latex_exp_str_tmp[1])-4] = '\0';
            }

            free(latex_exp_str[0]);
            free(latex_exp_str[1]);

            latex_exp_str[0] = latex_exp_str_tmp[0];
            latex_exp_str[1] = latex_exp_str_tmp[1];
        }
    }

    double final_ans = ans[0] / (ans[0] + ans[1]);

    printf("\nAnswer = %s / (%s + %s)\n       = %g\n\n",
           ans_str[0], ans_str[0], ans_str[1], final_ans);

    free(ans_str[0]);
    free(ans_str[1]);
    free(ans_str);
    free(ans_str_tmp);


    if (SHOWPI) {
        printf("\\[ \\frac{%s}{%s + %s} ",
               latex_pi_str[0], latex_pi_str[0], latex_pi_str[1]);
        
        free(latex_pi_str[0]);
        free(latex_pi_str[1]);
        free(latex_pi_str);
        free(latex_pi_str_tmp);
    }


    char *fin_ans_str;
    asprintf(&fin_ans_str, "%g", final_ans);

    // Replace the 'e' character in the answer
    char *pre_e[2] = {strtok(fin_ans_str, "e"), strtok(NULL, "e")};
    char *equal_or_approx = pre_e[1] ? "\\approx" : "=";
    if (pre_e[1]) {
        char *ans_tmp;
        asprintf(&ans_tmp, "%s \\times 10^{%s}", pre_e[0], pre_e[1]);
        free(fin_ans_str);
        fin_ans_str = ans_tmp;
    }


    if (SHOWEXP) {
        printf("%s \\frac{%s}{%s + %s} %s %s \\]\n",
               SHOWPI ? "=" : "\\[",
               latex_exp_str[0], latex_exp_str[0], latex_exp_str[1],
               equal_or_approx, fin_ans_str);

        free(latex_exp_str[0]);
        free(latex_exp_str[1]);
        free(latex_exp_str);
        free(latex_exp_str_tmp);
    } else {
        printf("%s %s \\]\n", equal_or_approx, fin_ans_str);
    }
    free(fin_ans_str);
}
