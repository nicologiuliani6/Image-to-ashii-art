#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <armadillo>
#include <cairo/cairo.h>

#define txt false //set to true if you want a txt file with a black/white of the art
//IMAGE TO ASHII -NG
//how to use:      ./main [file_name][TRANSPARENT_LAYER][REDUCE_SIZE]
//note:            TRANSPARENT_LAYER: 256 full color
//note:            0<=TRANSPARENT_LAYER<=255 for specific level
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr
            << "missing param.: [file_path] [lvl_transparency] [pixel_scale]\n"
            << " - [file_path]: path of the file\n"
            << " - [lvl_transparency]: skip the pixel over this level of color\n"
            << " - [pixel_scale]: 1 for original pixel size, 2 for half, 4 for a quarter, ecc\n";
        return 1;
    }
    const int REDUCE = (int)std::atoi(argv[3]);
    int width, height, channels;
    unsigned char* data = stbi_load(argv[1], &width, &height, &channels, 4); 
    if (!data) {
        std::cerr << "Error: impossible to open the image\n";
        return 1;
    }
    std::ofstream file("output.txt");  
    if (txt) if (!file) {
        std::cerr << "Error: impossible to create or open the file\n";
        return 1;
    }
    //calcolo matrice con pixel side ridotto
    //[width][height]
    arma::Mat<int> R(width / REDUCE, height / REDUCE, arma::fill::zeros);  
    arma::Mat<int> G(width / REDUCE, height / REDUCE, arma::fill::zeros);     
    arma::Mat<int> B(width / REDUCE, height / REDUCE, arma::fill::zeros);     
    for(auto x=0; x<width; x+=REDUCE){
        for(auto y=0; y<height; y+=REDUCE){
            if (x+REDUCE<=width && y+REDUCE <= height) {
                for(auto i=0; i<REDUCE; i++){
                     R(x / REDUCE, y / REDUCE) += ((int)(data + ((y+i) * width + (x+i)) * 4)[0]); //R
                     G(x / REDUCE, y / REDUCE) += ((int)(data + ((y+i) * width + (x+i)) * 4)[1]); //G 
                     B(x / REDUCE, y / REDUCE) += ((int)(data + ((y+i) * width + (x+i)) * 4)[2]); //B
                }
                R(x/REDUCE, y/REDUCE)/=REDUCE; //R
                G(x/REDUCE, y/REDUCE)/=REDUCE; //G
                B(x/REDUCE, y/REDUCE)/=REDUCE; //B
            }
        }
    }
    //stampa a 90 gradi
    const std::string characters = "@%# ";
    arma::Mat<int> R_rotated = arma::trans(R);
    arma::Mat<int> G_rotated = arma::trans(G);
    arma::Mat<int> B_rotated = arma::trans(B);
    // Creazione immagine PNG con trasparenza
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // Sfondo trasparente
    cairo_set_source_rgba(cr, 0, 0, 0, 0); // RGBA: tutto 0 = completamente trasparente
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);

    cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, REDUCE); // ogni carattere grande quanto il blocco ridotto

    for (auto y = 0; y < height / REDUCE; ++y) {
        for (auto x = 0; x < width / REDUCE; ++x) {
            int avg = (R_rotated(y, x) + G_rotated(y, x) + B_rotated(y, x)) / 3;

            if (avg < std::atoi(argv[2])) {
                // Colore carattere
                double r = R_rotated(y, x) / 255.0;
                double g = G_rotated(y, x) / 255.0;
                double b = B_rotated(y, x) / 255.0;
                cairo_set_source_rgb(cr, r, g, b);

                // Carattere
                char ch[2] = { characters[ avg * (characters.size() - 1) / 255 ], '\0' };
                cairo_move_to(cr, x * REDUCE, y * REDUCE + REDUCE);
                cairo_show_text(cr, ch);
            }
        }
    }

    // Salvataggio con alpha
    cairo_surface_write_to_png(surface, "ascii.png");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);


    stbi_image_free(data);
    file.close(); 
    return 0;
}

