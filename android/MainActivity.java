package lol.otp;

import java.util.List;
import java.util.Locale;

import android.support.v7.app.ActionBarActivity;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

/**
 *
 * @author ganich_j
 *
 */
public class MainActivity extends ActionBarActivity {

	private TextView tv_cd;
	private ListView lv_accountList;
	private EditText et_account;
	private EditText et_token;
	
	private AccountsDataSource	dataSource;

	private LayoutInflater li;
	private View promptsView;

	private AccountListAdapter adapter;

	private int secondsLeft = 0;

	private List<Account>	myList;
	private CountDownTimer	firstCountDown;
	private CountDownTimer	normalTimer;

	private long countDownTime;

	private AlertDialog alertDialog;

	public void updateTokens() {
		for (Account acc : myList) {
			String code = TOTPUtility.generateDaCode(acc.getDecodedSecret());
			acc.setCode(code);
		}
		adapter.notifyDataSetChanged();
	}

	public void generateDialog() {

		AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);

		// set prompts.xml to alertdialog builder
		alertDialogBuilder.setView(promptsView);

		// set dialog message
		alertDialogBuilder
				.setCancelable(false)
				.setPositiveButton("Add",
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								// Create, add and update
								if (et_account.getText().toString().length() > 0 &&
									et_token.getText().toString().length() > 0 &&
									et_token.getText().toString().length() == 32) {
									Account tmp = dataSource.createAccount(
											et_account.getText().toString(),
											et_token.getText().toString().replaceAll("\\s+","").toUpperCase(Locale.US));
									myList.add(tmp);
									updateTokens();
								}
							}
						})
				.setNegativeButton("Cancel",
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								dialog.cancel();
							}
						});

		// create alert dialog
		alertDialog = alertDialogBuilder.create();
	}

	@SuppressLint("InflateParams")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		dataSource = new AccountsDataSource(this);
		dataSource.open();
		myList = dataSource.getAllAccounts();
		li = LayoutInflater.from(this);
		promptsView = li.inflate(R.layout.dialog, null);
		//myList.add(new Account("Léo", "6yof 6u6b pvgp j34k 2zc5 jzl2 d3eu bjmr"));
		et_account = (EditText) promptsView.findViewById(R.id.et_account_name);
		et_token = (EditText) promptsView.findViewById(R.id.et_token);
		lv_accountList = (ListView) findViewById(R.id.list);
		tv_cd = (TextView) findViewById(R.id.countdown);
		adapter = new AccountListAdapter(this, R.layout.list_row, myList);
		lv_accountList.setAdapter(adapter);
		countDownTime = TOTPUtility.getTimeTillNextTick(TOTPUtility
				.getUnixTime());
		tv_cd.setText(String.valueOf(countDownTime));
		generateDialog();
		firstCountDown = new CountDownTimer(countDownTime * 1000, 100) {

			@Override
			public void onTick(long millisUntilFinished) {
				if (Math.round((float) millisUntilFinished / 1000.0f) != secondsLeft) {
					secondsLeft = Math
							.round((float) millisUntilFinished / 1000.0f);
				}
				countDownTime = TOTPUtility.getTimeTillNextTick(TOTPUtility
						.getUnixTime());
				tv_cd.setText(String.valueOf(countDownTime));
			}

			@Override
			public void onFinish() {
				countDownTime = TOTPUtility.getTimeTillNextTick(TOTPUtility
						.getUnixTime());
				tv_cd.setText(String.valueOf(countDownTime));
				updateTokens();
				normalTimer.start();
			}
		};
		normalTimer = new CountDownTimer(TOTPUtility.TIME_STEP * 1000, 100) {

			@Override
			public void onTick(long millisUntilFinished) {
				if (Math.round((float) millisUntilFinished / 1000.0f) != secondsLeft) {
					secondsLeft = Math
							.round((float) millisUntilFinished / 1000.0f);
				}
				countDownTime = TOTPUtility.getTimeTillNextTick(TOTPUtility
						.getUnixTime());
				tv_cd.setText(String.valueOf(countDownTime));
			}

			@Override
			public void onFinish() {
				countDownTime = TOTPUtility.getTimeTillNextTick(TOTPUtility
						.getUnixTime());
				tv_cd.setText(String.valueOf(countDownTime));
				updateTokens();
				this.start();
			}
		};
		updateTokens();
		firstCountDown.start();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		int id = item.getItemId();
		if (id == R.id.action_add) {
			alertDialog.show();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
