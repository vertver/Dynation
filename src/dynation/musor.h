#if 0
        if (SpecialAspectWH < 75 * 0.01) {  // если ширина плагина меньше каковото размера то отключаем блок
            f32 BigSizeScale = (2098/1049); // 

            Width2BloockPercent[0] *= BigSizeScale;
            Width2BloockPercent[1] *= BigSizeScale;
            Width2BloockPercent[3] *= BigSizeScale;
            Width2BloockPercent[4] = 0;
            Width2BloockPercent[5] = 0;
            Width2BloockPercent[6] *= BigSizeScale;

            Width2BloockPercent[2] = 1.0 - Width2BloockPercent[0] -Width2BloockPercent[1] - Width2BloockPercent[3] 
                - Width2BloockPercent[4] - Width2BloockPercent[5] - Width2BloockPercent[6];

            HideCompessor = true;
        } else {
            HideCompessor = false;

            // чтобы левая часть не становилась меньше начального размера зжимаем правую часть
            f32 MainBlockAspect         =  Width2BloockPercent[2] * Width1Bloock[1].Size.x / Width1Bloock[1].Size.y;
            f32 MainBlockAspectNead     =  (798.0/1154.0);
            if (MainBlockAspect < MainBlockAspectNead) { 
                f32 OffsetPercent       =  MainBlockAspectNead/MainBlockAspect; // относительно 
                Width2BloockPercent[2]  *= OffsetPercent; 
                Width2BloockPercent[0]  *= OffsetPercent; 
                Width2BloockPercent[6]  *= OffsetPercent; 

                Width2BloockPercent[4]  = 1.0 - Width2BloockPercent[0] -Width2BloockPercent[1] - Width2BloockPercent[2] 
                    - Width2BloockPercent[3] - Width2BloockPercent[5] - Width2BloockPercent[6];
            }
        }
#endif