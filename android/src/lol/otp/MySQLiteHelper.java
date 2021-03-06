package lol.otp;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class MySQLiteHelper extends SQLiteOpenHelper {

	public static final String TABLE_ACCOUNTS 	= "accounts";
	public static final String COLUMN_ID 		= "_id";
	public static final String COLUMN_MAIL		= "accountName";
	public static final String COLUMN_TOKEN		= "originalSecret";
	
	private static final String DATABASE_NAME	= "accounts.db";
	private static final int DATABASE_VERSION 	= 1;

	// Database creation sql statement
	  private static final String DATABASE_CREATE = "create table "
	      + TABLE_ACCOUNTS + "("					+ COLUMN_ID
	      + " integer primary key autoincrement, "	+ COLUMN_MAIL
	      + " text not null, " 						+ COLUMN_TOKEN
	      + " text not null)";
	
	public MySQLiteHelper(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		db.execSQL(DATABASE_CREATE);
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		db.execSQL("DROP TABLE IF EXISTS " + TABLE_ACCOUNTS);
	    onCreate(db);
	}

}
