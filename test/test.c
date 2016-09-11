#include "test.h"
#include "../src/utils/error.h"
#include "../src/utils/string.h"

int main(int argc, char * argv[])
{
    if(argc <= 4)
    {
        fail("You need at least five arguments");
    }

    char * bir = argv[1];
    char * cc = str_con(2, argv[2], " ");
    char * cfg = argv[3];
    char * src = str_con(2, " ", argv[4]);
    char * file_path = str_con(2, bir, "test.xml");

    char * dbg = "gdb ";
    char * dfg1 = "-ex \"set confirm off\" -ex \"run\"";
    char * dfg2 = " -ex \"bt full\" -ex \"kill\" -ex \"quit\" -q";

    FILE * xml = fopen(file_path, "w");
    fprintf(xml, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    fprintf(xml, "<testsuites>");

    for(int i = 5; i < argc; ++i)
    {
        clock_t start, stop;
        start = clock();
        fprintf(xml, "<testsuite name=\"%s\" ", argv[i]);

        char str[4096];
        char * cc_cmd;
        cc_cmd = str_con(7, cc, cfg, " -o ", bir, "test.o ", argv[i], src);
        char * tmp = str_con(2, cc_cmd, " 2>&1");
        free(cc_cmd);
        cc_cmd = tmp;
        char * cc_out = calloc(1, 1);
        FILE * cc_f = popen(cc_cmd, "r");
        while(fgets(str, sizeof(str) - 1, cc_f) != NULL)
        {
            printf("cc -> %s", str);
            char * tmp = str_con(2, cc_out, str);
            free(cc_out);
            cc_out = tmp;
        }
        int cc_code = pclose(cc_f) / 256;
        printf("cc -> %s exited with code : %d\n", cc, cc_code);
        free(cc_cmd);
        
        char * pg_out = calloc(1, 1);
        char * pg_cmd = str_con(2, bir, "test.o 2>&1");
        FILE * pg_f = popen(pg_cmd, "r");
        while(fgets(str, sizeof(str) - 1, pg_f) != NULL)
        {
            printf("pg -> %s", str);
            char * tmp = str_con(2, pg_out, str);
            free(pg_out);
            pg_out = tmp;
        }
        int pg_code = pclose(pg_f) / 256;
        printf("pg -> test exited with code : %d\n", pg_code);
        free(pg_cmd);

        char * dbg_out = calloc(1, 1);
        if(pg_code != 0)
        {
            char * dbg_cmd = str_con(6, dbg, bir, "test.o ", dfg1, dfg2, " 2>&1");
            FILE * dbg_f = popen(dbg_cmd, "r");
            while(fgets(str, sizeof(str) - 1, dbg_f) != NULL)
            {
                printf("dg -> %s", str);
                char * tmp = str_con(2, dbg_out, str);
                free(dbg_out);
                dbg_out = tmp;
            }
            int dbg_code = pclose(dbg_f) / 256;
            printf("db -> gdb exited with code : %d\n", dbg_code);
            free(dbg_cmd);
        }

        stop = clock();
        double tps = (stop - start) / CLOCKS_PER_SEC;
        int e_nb = cc_code == 0 ? 0 : 1;
        int f_nb = pg_code == 0 ? 0 : 1;
        fprintf(xml, "errors=\"%d\" failures=\"0\" tests=\"1\">", e_nb + f_nb);
        fprintf(xml, "<testcase name=\"%s\" time=\"%f\">", argv[i], tps);
        fprintf(xml, "<system-out>%s</system-out>", pg_out);
        fprintf(xml, "<system-err>%s\n\n%s</system-err>", cc_out, dbg_out);
        fprintf(xml, "</testcase>");
        fprintf(xml, "</testsuite>");

        free(cc_out);
        free(pg_out);
        free(dbg_out);
    }
    free(cc);
    free(src);
    free(file_path);

    fprintf(xml, "</testsuites>");
    fclose(xml);

    return 0;
}
