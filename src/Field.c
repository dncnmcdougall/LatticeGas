#include <stdlib.h>
#include <stdio.h>
#include <error.h>

#include "png.h"

#include "Field.h"


Field* createField(int nRows, int nCols){
    Field* pField = malloc(sizeof(Field));
    pField->nRows = nRows;
    pField->nCols = nCols;
    pField->field = calloc(nRows, sizeof(int*));
    int r = 0;
    for ( r = 0; r < nRows; ++r) {
        pField->field[r] = calloc(nCols, sizeof(int));
    }
    return pField;
}

void freeField(Field* pField) {
    int r = 0;
    for ( r = 0; r < pField->nRows; ++r) {
        free(pField->field[r]);
    }
    free(pField->field);
    free(pField);
    pField = NULL;
}

void printFieldInHex(const Field* pField) {
    int r = 0, c = 0;
    for ( r = 0; r < pField->nRows; ++r) {
        if ( (r & 1) == 0 ) {
            printf(" ");
        }
        for ( c = 0; c < pField->nCols; ++c) {
            printf("%i ",pField->field[r][c]);
        }
        printf("\n");
    }
}

unsigned char getMomentumValue( const Vector* pMom, int r, int c) {

    int thresh = 60;

    int x = pMom->x;
    int y = pMom->y;

    switch(r) {
        case 0:
            if ( y > thresh ) {
                switch(c) {
                    case 0: return (x < -thresh) ? 0 : 255;
                    case 1: return (x < -0 && x >= -thresh ) ? 0 : 255;
                    case 2: return (x == 0) ? 0 : 255;
                    case 3: return (x > 0 && x <= thresh) ? 0 : 255;
                    case 4: return (x > thresh) ? 0 : 255;
                }
            } else {
                return 255;
            }
        case 1:
            if ( y > 0 ) {
                switch(c) {
                    case 0: return (x < -thresh) ? 0 : 255;
                    case 1: return (x < -0 ) ? 0 : 255;
                    case 2: return (x == 0) ? 0 : 255;
                    case 3: return (x > 0 ) ? 0 : 255;
                    case 4: return (x > thresh) ? 0 : 255;
                }
            } else {
                return 255;
            }
        case 2:
            switch(c) {
                case 0: return (y == 0 && x < -thresh) ? 0 : 255;
                case 1: return (y == 0 && x < -0) ? 0 : 255; 
                case 2: return 0; 
                case 3: return (y == 0 && x > 0) ? 0 : 255; 
                case 4: return (y == 0 && x > thresh) ? 0 : 255;
            } 
        case 3: 
            if ( y < 0  ) { 
                switch(c) { 
                    case 0: return (x < -thresh) ? 0 : 255;
                    case 1: return (x < -0 ) ? 0 : 255;
                    case 2: return (x == 0) ? 0 : 255;
                    case 3: return (x > 0 ) ? 0 : 255;
                    case 4: return (x > thresh) ? 0 : 255;
                } 
            } else { 
                return 255; 
            } 
        case 4: 
            if ( y < -thresh ) { 
                switch(c) { 
                    case 0: return (x < -thresh) ? 0 : 255;
                    case 1: return (x < -0 && x >= -thresh ) ? 0 : 255;
                    case 2: return (x == 0) ? 0 : 255;
                    case 3: return (x > 0 && x <= thresh) ? 0 : 255;
                    case 4: return (x > thresh) ? 0 : 255;
                } 
            } else { 
                return 255; 
            } 
    } 
    error(0,1,"THIS SHOULD NOT HAPPEN\n"); return 0; 
} 
png_bytepp computeRows(Field* pField, 
                       Vector (*computeMomentum)(int,int,Field*),
                       CellType (*getCellType)(int,int)
                       ) {
    int nPixRows = pField->nRows*5; 
    int nPixCols = pField->nCols*5; 
    // printf("CRows: rows %i, cols %i\n", nPixRows, nPixCols);

    int pixR, pixC;
    int cellR, cellC;
    int i, j;
    Vector cellMom;
    CellType cellT;

    png_bytepp pRows = malloc(sizeof(png_bytep)*nPixRows);

    for(cellR=0; cellR<pField->nRows; ++cellR) {
        for(cellC=0; cellC<pField->nCols; ++cellC) {
            cellMom = computeMomentum(cellR, cellC, pField);
            cellT = getCellType(cellR, cellC);
            for( i = 0; i < 5; ++i ) {
                pixR = cellR*5 + i;
                if ( cellC == 0 ) {
                    pRows[pixR] = malloc(sizeof(png_byte)*nPixCols);
                }
                for ( j = 0; j < 5; ++j ) {
                    pixC = cellC*5+j;
                    pRows[pixR][pixC] = getMomentumValue(&cellMom, i, j);
                    switch( cellT ) {
                        case Fixed:
                            pRows[pixR][pixC] = pRows[pixR][pixC] & 210;
                            break;
                        case Mirror:
                            pRows[pixR][pixC] = pRows[pixR][pixC] & 100;
                            break;
                        case Flow:
                            break;
                    }
                    // if ( cellR == cellC ) printf("value: (%i, %i) : %i\n", i, j, pRows[pixR][pixC]);
                }
            }
        }
    }

    return pRows;
}

void pngWarningCallback (png_structp pPng, png_const_charp png_error) {
    printf("PNG WARN: %s\n", png_error);
}

void pngErrorCallback (png_structp pPng, png_const_charp png_error) {
    error(0,1,"PNG ERROR: %s\n", png_error);
}



void drawField(Field* pField, const char* filename,
               Vector (*computeMomentum)(int,int,Field*),
               CellType (*getCellType)(int,int)
              ) {

    png_structp pPng = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, NULL, 
        pngErrorCallback, pngWarningCallback);

    if(!pPng) {
        error(0,1,"Could not allocate the png struct.\n");
    }

    png_infop pPngInfo = png_create_info_struct(pPng);

    if(!pPngInfo) {
        png_destroy_write_struct(&pPng,NULL);
        error(0,1,"Could not allocate the png info struct.\n");
    }

    FILE* fp = fopen(filename,"wb");
    if (!fp) {
        error(0,1, "File could not be opened for writing");
    }

    png_init_io(pPng, fp);

    int nPixRows = pField->nRows*5;
    int nPixCols = pField->nCols*5;

    png_set_IHDR(pPng,
                 pPngInfo, nPixRows, nPixCols, 8,
                 PNG_COLOR_TYPE_GRAY, 
                 PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    png_bytepp pRows = computeRows(pField, computeMomentum, getCellType);
    png_set_rows(pPng, pPngInfo, pRows);

    png_write_png(pPng, pPngInfo, PNG_TRANSFORM_IDENTITY, NULL);

    png_write_end(pPng, pPngInfo);
    png_destroy_write_struct(&pPng, &pPngInfo);

    fclose(fp);
}
