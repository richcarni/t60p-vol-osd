#include <alsa/asoundlib.h>

void SetAlsaMasterVolume(long volume)
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}

long GetAlsaMasterVolume()
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default:29";
    const char *selem_name = "Console";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    printf("min %i max %i\n",min, max);
    long vol = 0;
    snd_mixer_selem_get_playback_volume(elem,1,&vol);

    snd_mixer_close(handle);
    return vol;
}

int volume_change_callback(snd_mixer_elem_t* elem, unsigned int mask) {
    long vol = 0;
    snd_mixer_selem_get_playback_volume(elem,1,&vol);

    char cmd[100] = "echo ";
    char* str2 = " | /usr/bin/gdbar -l Volume -w 50 -h 7 | /usr/bin/dzen2 -p 2 -ta c -y 870 -x 1300 -tw 140&";
    char str1[4];
    sprintf(str1, "%i", vol*100/14);

    
    strcat(cmd, str1);
    strcat(cmd, str2);
    system(cmd);
}

void monitorVolume() {
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default:29";
    const char *selem_name = "Console";
    
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_elem_set_callback(elem, volume_change_callback); 

    while (1) {
      int res;
      res = snd_mixer_wait(handle, -1);
      if (res >= 0) {
          res = snd_mixer_handle_events(handle);
          assert(res >= 0);
      }
    }

}

int main(int argc, char const *argv[])
{
    monitorVolume();
    return 0;
}
