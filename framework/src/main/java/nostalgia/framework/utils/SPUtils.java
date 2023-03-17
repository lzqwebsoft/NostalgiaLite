package nostalgia.framework.utils;

import android.content.Context;
import android.content.SharedPreferences;

public class SPUtils {
    private SharedPreferences shared;
    private SharedPreferences.Editor editor;

    public SPUtils(Context context) {
        shared = context.getSharedPreferences("TempData", Context.MODE_PRIVATE);
        editor = shared.edit();
    }

    public String getString(String key, String value) {
        try {
            return shared.getString(key, value);
        } catch (Exception e) {
            editor.remove(key);
            editor.commit();
            return value;
        }
    }

    public Boolean getBoolean(String key, Boolean value) {
        try {
            return shared.getBoolean(key, value);
        } catch (Exception e) {
            editor.remove(key);
            editor.commit();
            return value;
        }
    }

    public int getInt(String key, int value) {
        try {
            return shared.getInt(key, value);
        } catch (Exception e) {
            editor.remove(key);
            editor.commit();
            return value;
        }
    }

    public double getDouble(String key, Double value) {
        try {
            return Double.parseDouble(shared.getString(key, value.toString()));
        } catch (Exception e) {
            editor.remove(key);
            editor.commit();
            return value;
        }
    }

    public float getFloat(String key, Float value) {
        try {
            return shared.getFloat(key, value);
        } catch (Exception e) {
            editor.remove(key);
            editor.commit();
            return value;
        }
    }

    public void setString(String key, String value) {
        editor.putString(key, value);
        editor.commit();
    }

    public void setBoolean(String key, Boolean value) {
        editor.putBoolean(key, value);
        editor.commit();
    }

    public void setInt(String key, int value) {
        editor.putInt(key, value);
        editor.commit();
    }

    public void setFloat(String key, Float value) {
        editor.putFloat(key, value);
        editor.commit();
    }

    public void setDouble(String key, Double value) {
        editor.putString(key, value.toString());
        editor.commit();
    }

    public boolean contains(String key) {
        return shared.contains(key);
    }

    public void remove(String key) {
        editor.remove(key);
        editor.commit();
    }

    public void clear() {
        editor.clear();
        editor.commit();
    }
}
