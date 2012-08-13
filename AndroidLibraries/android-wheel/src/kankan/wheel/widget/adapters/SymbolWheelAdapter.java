/*
 *  Copyright 2012 YH, Yang <yhuiyang@gmail.com>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

package kankan.wheel.widget.adapters;

import android.content.Context;

/**
 * Symbols Wheel adapter.
 */
public class SymbolWheelAdapter extends AbstractWheelTextAdapter {

    /** The default symbol */
    private static final String DEFAULT_SYMBOLS = "!@#$%^&*()";
   
    // Symbols
    private String symbols;
       
    /**
     * Constructor
     * @param context the current context
     */
    public SymbolWheelAdapter(Context context) {
        this(context, DEFAULT_SYMBOLS);
    }

    /**
     * Constructor
     * @param context the current context
     * @param symbols the symbols
     */
    public SymbolWheelAdapter(Context context, String symbols) {
        super(context);
        this.symbols = symbols;
    }

    @Override
    public int getItemsCount() {
        return symbols.length();
    }

    @Override
    protected CharSequence getItemText(int index) {
        if (index < symbols.length())
            return symbols.subSequence(index, index + 1);
        else
            return null;
    }
}
