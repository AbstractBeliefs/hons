#include "common.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

vs_heightmap_t heightmap_from_array(uint32_t rows, uint32_t cols, int32_t *input){
    vs_heightmap_t heightmap;

    heightmap.rows = rows;
    heightmap.cols = cols;
    heightmap.cellsize = 1;

    heightmap.corner = true;
    heightmap.xll = 0;
    heightmap.yll = 0;

    heightmap.nodata = -9999;
    heightmap.heightmap = calloc(rows * cols, sizeof(int32_t));

    memcpy(heightmap.heightmap, input, rows*cols*sizeof(int32_t));

    return heightmap;
}

vs_heightmap_t heightmap_from_file(FILE* inputfile){
    vs_heightmap_t map;
    
    map.rows = 0;
    map.cols = 0;
    map.cellsize = 1;

    map.corner = true;
    map.xll = 0;
    map.yll = 0;

    map.nodata = -9999;

    // Get the file length
    fseek(inputfile, 0, SEEK_END);
    size_t fsize = ftell(inputfile);
    rewind(inputfile);

    // Allocate buffers
    char* buffer = calloc(fsize, sizeof(char)); // TODO: needs errorchecked
    size_t result = fread(buffer, 1, fsize, inputfile); // TODO: needs errorchecked for != fsize
    buffer[fsize+sizeof(char)-1] = '\0';
    
    // Parse file
    bool r_parse = false;
    bool c_parse = false;
    size_t cellcount = 0;

    char* fragment = strtok(buffer, " \n");
    while (fragment != NULL){
        if (!strcmp(fragment, "NROWS")){
            fragment = strtok(NULL, " \n"); map.rows = atoi(fragment);
            if ((r_parse = true) && c_parse){ map.heightmap = calloc(map.rows * map.cols, sizeof(int32_t)); }
        }
        else if (!strcmp(fragment, "NCOLS")){
            fragment = strtok(NULL, " \n"); map.cols = atoi(fragment);
            if ((c_parse = true) && r_parse){ map.heightmap = calloc(map.rows * map.cols, sizeof(int32_t)); }
        }
        else if (!strcmp(fragment, "XLLCORNER") || !strcmp(fragment, "XLLCENTER")){
            if (!strcmp(fragment, "XLLCORNER")){ map.corner = true; } else { map.corner = false; }
            fragment = strtok(NULL, " \n"); map.xll = atoi(fragment);
        }
        else if (!strcmp(fragment, "YLLCORNER") || !strcmp(fragment, "YLLCENTER")){
            if (!strcmp(fragment, "YLLCORNER")){ map.corner = true; } else { map.corner = false; }
            fragment = strtok(NULL, " \n"); map.yll = atoi(fragment);
        }
        else if (!strcmp(fragment, "CELLSIZE")){ fragment = strtok(NULL, " \n"); map.cellsize = atoi(fragment); }
        else if (!strcmp(fragment, "NODATA_VALUE")){ fragment = strtok(NULL, " \n"); map.nodata = atoi(fragment); }
        else {
            if (map.heightmap && cellcount < map.cols*map.rows){
                map.heightmap[cellcount++] = atoi(fragment);
            }
        }

        fragment = strtok(NULL, " \n");
    }
    
    return map;
}


vs_viewshed_t viewshed_from_array(uint32_t rows, uint32_t cols, bool *input){
    vs_viewshed_t viewshed;

    viewshed.rows = rows;
    viewshed.cols = cols;
    viewshed.cellsize = 1;

    viewshed.corner = true;
    viewshed.xll = 0;
    viewshed.yll = 0;

    viewshed.viewshed = calloc(rows * cols, sizeof(int32_t));

    memcpy(viewshed.viewshed, input, rows*cols*sizeof(int32_t));

    return viewshed;
}

void viewshed_to_file(vs_viewshed_t viewshed, FILE* outputfile){
    fprintf(outputfile, "NCOLS %d\n", viewshed.cols);
    fprintf(outputfile, "NROWS %d\n", viewshed.rows);
    fprintf(outputfile, "XLL%s %d\n", (viewshed.corner ? "CORNER" : "CENTER"), viewshed.xll);
    fprintf(outputfile, "YLL%s %d\n", (viewshed.corner ? "CORNER" : "CENTER"), viewshed.yll);
    fprintf(outputfile, "CELLSIZE %d\n", viewshed.cellsize);

    for (size_t row = 0; row < viewshed.rows; row++){
        for (size_t col = 0; col < viewshed.cols; col++){
            fprintf(outputfile, "%d ", viewshed.viewshed[row*viewshed.cols+col]);
        }
        fseek(outputfile, -1, SEEK_CUR);
        fprintf(outputfile, "\n");
    }
    fflush(outputfile);
    return;
}

vs_viewshed_t viewshed_from_heightmap(vs_heightmap_t heightmap){
    vs_viewshed_t viewshed;

    viewshed.rows = heightmap.rows;
    viewshed.cols = heightmap.cols;
    viewshed.cellsize = heightmap.cellsize;

    viewshed.corner = heightmap.corner;
    viewshed.xll = heightmap.xll;
    viewshed.yll = heightmap.yll;

    viewshed.viewshed = calloc(viewshed.rows * viewshed.cols, sizeof(bool));

    return viewshed;
}

