
// Generated file - DO NOT EDIT!

#pragma once

class Sprite {
public:
    using OnEndAnimationCallback = void (*)(Sprite*);

    OnEndAnimationCallback onEndAnimation = +[](Sprite *sprite){ sprite->frame = 0; };
    const uint8_t *data = nullptr;
    std::uint32_t frameTime = 0;
    std::uint16_t frame = 0;
    std::uint16_t animation = 0;

    Sprite() = default;
    virtual ~Sprite(){}
    
    template<typename SpriteType>
    void play(const SpriteType& data, typename SpriteType::Animation animation){ 
        this->animation = static_cast<std::uint16_t>(animation); 
        this->data = reinterpret_cast<const uint8_t *>(&data);
        frame = 0;
        frameTime = Pokitto::Core::getTime() + getFrameDuration(0);
    }
    
    std::uint8_t getAnimationFrameCount(){ return data[ 2 + animation ]; }

    std::uint8_t getFrameId(std::uint32_t frame){ return data[ 2 + animation + 1 + frame * 2]; }

    std::uint32_t getFrameDuration(std::uint32_t frame){ return data[ 2 + animation + 1 + frame * 2 + 1 ] * 10; }
    
    std::uint32_t getFrameWidth(){ return getFrameBitmap(getFrameId(frame))[2]; }
    
    std::uint32_t getFrameHeight(){ return getFrameBitmap(getFrameId(frame))[3]; }

    const uint8_t *getFrameBitmap(std::uint32_t frameId){ 
        std::uint32_t offset = 2 + data[0] + (static_cast<std::uint32_t>(data[1]) << 8);
        uint32_t shift;
        switch(Pokitto::Display::m_colordepth){
        case 8: shift = 0; break;
        case 4: shift = 1; break;
        case 2: shift = 2; break;
        case 1: shift = 3; break;
        default: shift = 0; break;
        }
        
        while(frameId--){
            std::uint32_t w = data[offset + 2];
            std::uint32_t h = data[offset + 3];
            std::uint32_t total = w*h >> shift;
            offset += total + 4;
        }
        
        return data + offset;
    }

    void draw(std::uint32_t x, std::uint32_t y, bool flip=false, bool mirror=false, std::uint32_t recolor=0){
        if(!data) 
            return;
            
        std::uint32_t now = Pokitto::Core::getTime();
        while(true) {
            int32_t delta = now - frameTime;
            if( delta < 0 ){
                const uint8_t *bmp = getFrameBitmap(getFrameId(frame));
                x += *bmp++;
                y += *bmp++;

                #if PROJ_SCREENMODE == TASMODE
                if(Pokitto::Display::m_colordepth == 8){
                    Pokitto::Display::drawSprite(x, y, bmp, flip, mirror, recolor);
                    return;
                }
                #endif

                if(flip) Pokitto::Display::drawBitmapYFlipped(x, y, bmp);
                else if(mirror)  Pokitto::Display::drawBitmapXFlipped(x, y, bmp);
                else  Pokitto::Display::drawBitmap(x, y, bmp);
                return;
            }
            frame++;
            if( frame == getAnimationFrameCount() )
                onEndAnimation(this);
            frameTime += getFrameDuration(frame);
        }
    }
};

class SteamDriller_BigCog_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Boss_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0,
		Move = 7,
		Attack = 20
	};
};

class SteamDriller_Chip_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Chip_Black_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Chip_Purple_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Chip_Red_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Coal_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Crystal_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Dock_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_EliteMechaSpider {
public:
    enum Animation : std::uint16_t {
		Idle = 0,
		Move = 11,
		Attack = 22
	};
};

class SteamDriller_Explosions_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Laser_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_PurpleSentinels_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0,
		Move = 13,
		Attack = 24
	};
};

class SteamDriller_Robot_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0,
		Scanning = 3,
		Leaning = 18,
		Lean = 25,
		Crouching = 28,
		Crouch = 39,
		Dying = 44,
		Death = 53,
		Offline = 56
	};
};

class SteamDriller_Ruby_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Sign_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_SmallCog_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Smoke_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Smoke_Wheels_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Spider_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0,
		Move = 9,
		Attack = 18
	};
};

class SteamDriller_TNT_Detonator_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Up_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0
	};
};

class SteamDriller_Worm_Anim {
public:
    enum Animation : std::uint16_t {
		Idle = 0,
		Move = 3,
		Attack = 20
	};
};


// https://stackoverflow.com/a/19591902
extern "C" {
extern const SteamDriller_BigCog_Anim steamDriller_BigCog_Anim;
extern const SteamDriller_Boss_Anim steamDriller_Boss_Anim;
extern const SteamDriller_Chip_Anim steamDriller_Chip_Anim;
extern const SteamDriller_Chip_Black_Anim steamDriller_Chip_Black_Anim;
extern const SteamDriller_Chip_Purple_Anim steamDriller_Chip_Purple_Anim;
extern const SteamDriller_Chip_Red_Anim steamDriller_Chip_Red_Anim;
extern const SteamDriller_Coal_Anim steamDriller_Coal_Anim;
extern const SteamDriller_Crystal_Anim steamDriller_Crystal_Anim;
extern const SteamDriller_Dock_Anim steamDriller_Dock_Anim;
extern const SteamDriller_EliteMechaSpider steamDriller_EliteMechaSpider;
extern const SteamDriller_Explosions_Anim steamDriller_Explosions_Anim;
extern const SteamDriller_Laser_Anim steamDriller_Laser_Anim;
extern const SteamDriller_PurpleSentinels_Anim steamDriller_PurpleSentinels_Anim;
extern const SteamDriller_Robot_Anim steamDriller_Robot_Anim;
extern const SteamDriller_Ruby_Anim steamDriller_Ruby_Anim;
extern const SteamDriller_Sign_Anim steamDriller_Sign_Anim;
extern const SteamDriller_SmallCog_Anim steamDriller_SmallCog_Anim;
extern const SteamDriller_Smoke_Anim steamDriller_Smoke_Anim;
extern const SteamDriller_Smoke_Wheels_Anim steamDriller_Smoke_Wheels_Anim;
extern const SteamDriller_Spider_Anim steamDriller_Spider_Anim;
extern const SteamDriller_TNT_Detonator_Anim steamDriller_TNT_Detonator_Anim;
extern const SteamDriller_Up_Anim steamDriller_Up_Anim;
extern const SteamDriller_Worm_Anim steamDriller_Worm_Anim;
}
