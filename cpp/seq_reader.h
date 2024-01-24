#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <optional>
#include "json.hpp"

namespace seq_reader {
  
  class seq 
  {
  public:
    seq(std::string path) 
    {
      in = fopen(path.c_str(), "rb") ;
      if (in == nullptr)
      {
        printf("seq_reader error: the file %s could not be opened.\n", path.c_str()) ; fflush(stdout) ; 
        return ; 
      }
      
      path = path.substr(0, path.size() - 3) + "log"  ;  
      std::ifstream flog(path);
      if (flog.is_open()==false)
      {
        printf("seq_reader error: log file %s could not be opened .\n", path.c_str()) ; fflush(stdout) ; 
        return ; 
      }
      try { flog >> log; flog.close() ; }
      catch(...)
      {
        printf("seq_reader error: log file is not a legal json file, here is what we already got:\n") ;
        std::cout << log ;
      } 
      
      w = log["detector"]["image_size"]["width"] ; 
      h = log["detector"]["image_size"]["height"] ; 
      nimg = log["detector"]["frames"].size() ; 
      
      buffer.resize(w*h) ; 
    }
    
    ~seq()
    {
      if (in != nullptr) fclose(in) ; 
    }

    //--------------------
    template <typename T>
    void read_next (std::vector<std::vector<T>> & res)
    {
      size_t r = fread(buffer.data(), 2, w*h, in) ; 
      if (r!=w*h) printf("seq_reader error: not enough bytes read %ld %d\n", r, w*h) ;
      
      res.resize(h, std::vector<T>(w)) ; 
      for (unsigned int j=0 ; j<w*h; j++)
        res[j/w][j%w]=static_cast<T>(buffer[j]) ;
    }
    //-------------------
    template <typename T>
    void read_all ( std::vector<std::vector<std::vector<T>>> & res)
    {
      res.clear() ; 
      while (!feof(in))
      {
        size_t r = fread(buffer.data(), 2, w*h, in) ; 
        if (r!=w*h) printf("seq_reader error: not enough bytes read %ld %d\n", r, w*h) ;
        else
        {
        std::vector<std::vector<T>> img(h, std::vector<T>(w)) ; 
        for (unsigned int j=0 ; j<w*h; j++)
          img[j/w][j%w]=static_cast<T>(buffer[j]) ;
        res.push_back(img) ;         
        }
      }
    }    
    //------------------
    template <typename T>
    void read_image(std::vector<std::vector<T>> & res, int n)
    {
      if (n<0 || n>nimg)
      {
        printf("seq_reader error: image number requested does not exist (%d/%d frames).\n", nimg, n) ; fflush(stdout) ; 
        res={} ;
        return ; 
      }
      else
      {
       fseek(in, w*h*2*n, SEEK_SET) ;  
       return read_next<T>(res) ; 
      }        
    }
    //----------------------
    
    
    // public variables
    int width() {return w ; }
    int height() {return h ; }
    int nb_images() {return nimg ; }
    
  private:
    FILE * in = nullptr ; 
    nlohmann::json log ; 
    int w=0, h=0 ; 
    int nimg=0 ; 
    std::vector<uint16_t> buffer ; 
    
//======================== This part is now for displaying ===============================
#ifdef SDL_h_
  public: 
    template<typename T>
    void show(const std::vector<std::vector<T>> &I, 
                     int scale=1, 
                     std::optional<T> minimum={}, std::optional<T> maximum={}, 
                     int delay=0,
                     int xoffset=0, int yoffset=0) 
    {
        if (!isinitialised)
        {
          initialise (w, h, scale) ; 
          basescale = scale ; 
          isinitialised=true ;
        }
        else
        {
          if (scale != basescale)
            printf("seq_reader::show error: the scale cannot be modified after the first call to this function") ; 
          scale=basescale ; 
        }
      
        // Update the texture with brightness data
        SDL_Color* pixels = nullptr;
        int pitch;
        SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);
        
        if (!maximum.has_value() || !minimum.has_value())
        {
          T maxtmp=I[0][0], mintmp=I[0][0] ; 
          for (auto & v:I)
            for (auto &w:v)
            {
                if (std::isnan(w)) continue ; 
                if (w>maxtmp)
                    maxtmp = w ; 
                if (w<mintmp)
                    mintmp = w ; 
            }
          if (!maximum.has_value()) maximum=maxtmp ; 
          if (!minimum.has_value()) minimum=mintmp ; 
        }

        for (size_t y = 0; y < I.size(); y+=scale) {
            for (size_t x = 0; x < I[0].size(); x+=scale) {
                SDL_Color color = mapBrightnessToColor<T>(I[y][x], maximum.value(), minimum.value());
                pixels[ (y /scale + yoffset) * (pitch / sizeof(Uint32)) + (x / scale + xoffset)] = color;
            }
        }

        SDL_UnlockTexture(texture);

        // Clear the renderer and render the texture
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);   
        
        if (delay>=0)
          SDL_Delay(delay) ; 
        else 
          wait_event() ; 
    }

  private: 
        SDL_Window* window ;
        SDL_Renderer* renderer ;
        SDL_Texture* texture ;
        bool isinitialised=false ; 
        int basescale=1 ; 
        
        // Function to map a brightness value to an RGB color
        template<typename T>
        SDL_Color mapBrightnessToColor(T brightness, T maximum, T minimum) 
        {
          if (brightness<minimum) brightness=minimum ; 
          if (brightness>maximum) brightness=maximum ;
          if (std::isnan(brightness)) return {0,255, 0, 0};
          Uint8 colorValue = static_cast<Uint8>( static_cast<double>(brightness - minimum)/(maximum-minimum) * 255.0);
          return {0,colorValue, colorValue, colorValue};  // R, G, B, Alpha
        }

        void initialise (int w, int h, int scale)
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
                return ;
            }
            
            int SCREEN_HEIGHT = h/scale ; 
            int SCREEN_WIDTH = w/scale ; 

            window = SDL_CreateWindow("Display", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
            
            if (!window || !renderer || !texture) {
                std::cerr << "SDL window, renderer, or texture creation failed: " << SDL_GetError() << std::endl;
                return ;
            }        

        }
        //------------------------
        void wait_event ()
        {
            bool quit = false;
            SDL_Event e;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                            // Clean up and quit SDL
                        SDL_DestroyTexture(texture);
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                    }
                    else if (e.type == SDL_KEYDOWN) {
                        quit = true;
                    }
                }
            }
            return ; 
        }
    
#endif
  } ;   
} 

