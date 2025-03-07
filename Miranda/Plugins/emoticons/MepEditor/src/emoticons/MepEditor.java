package emoticons;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.events.ControlAdapter;
import org.eclipse.swt.events.ControlEvent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.FontData;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

public class MepEditor
{
	private static MepFormat mepFormat;
	private static Mep mep;
	private static final List<String> protocols = new ArrayList<String>();
	static Shell shell;
	private static ScrolledComposite emoticonsScroll;
	private static Composite emoticonsComposite;
	private static Combo protoCombo;
	
	private static boolean inGuiSetCommand;
	
	public static void main(String[] args) throws IOException
	{
		List<Emoticon> emoticons = new ArrayList<Emoticon>();
		new EmoFormat(emoticons, protocols).load();
		
		Display display = new Display();
		
		shell = new Shell(display);
		DirectoryDialog dialog = new DirectoryDialog(shell, SWT.NONE);
		dialog.setText("Select folder with emoticon images");
		String path = dialog.open();
		if (path == null)
			return;
//		String path = "C:\\Desenvolvimento\\Miranda\\bin\\Debug Unicode\\Customize\\Emoticons\\Originals";
		File mepPath = new File(path);
		if (!mepPath.exists())
			return;
		
		mepFormat = new MepFormat(mepPath, emoticons, protocols);
		mep = mepFormat.load();
		
		createMainWindow(shell);
		
		shell.setText("Emoticon Pack Editor");
		shell.setImage(Images.get("data/Defaults/smile.png"));
		shell.setSize(500, 600);
		shell.open();
		
		while (!shell.isDisposed())
		{
			if (!display.readAndDispatch())
				display.sleep();
		}
		display.dispose();
	}
	
	static long time = System.nanoTime();
	
	@SuppressWarnings("unused")
	private static void profile(String text)
	{
		time = System.nanoTime() - time;
		
		System.out.println(text + " : " + (time / 1000000));
		
		time = System.nanoTime();
	}
	
	private static void createMainWindow(Composite main)
	{
		main.setLayout(new GridLayout(1, true));
		
		createToolbar(main);
		createPackGroup(main);
		createEmoticonsGroup(main, null);
	}
	
	private static void createToolbar(Composite main)
	{
		Button save = new Button(main, SWT.PUSH);
		save.setImage(Images.get("imgs/disk.png"));
		save.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event event)
			{
				try
				{
					mepFormat.save();
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		});
	}
	
	private static void createPackGroup(Composite main)
	{
		Group group = new Group(main, SWT.NONE);
		group.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		group.setText("Pack");
		group.setLayout(new GridLayout(2, false));
		
		createBoldLabel(group, "Name:");
		Text text = new Text(group, SWT.BORDER);
		text.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		text.setText(mep.name);
		text.addListener(SWT.Modify, new Listener() {
			public void handleEvent(Event e)
			{
				mep.name = ((Text) e.widget).getText();
			}
		});
		
		createBoldLabel(group, "Creator:");
		text = new Text(group, SWT.BORDER);
		text.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		text.setText(mep.creator);
		text.addListener(SWT.Modify, new Listener() {
			public void handleEvent(Event e)
			{
				mep.creator = ((Text) e.widget).getText();
			}
		});
		
		createBoldLabel(group, "Updater URL:");
		text = new Text(group, SWT.BORDER);
		text.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		text.setText(mep.udaterURL);
		text.addListener(SWT.Modify, new Listener() {
			public void handleEvent(Event e)
			{
				mep.udaterURL = ((Text) e.widget).getText();
			}
		});
		
	}
	
	private static void createEmoticonsGroup(final Composite main, String protocol)
	{
		final Group group = new Group(main, SWT.NONE);
		group.setLayoutData(new GridData(GridData.FILL_BOTH));
		group.setText("Emoticons");
		group.setLayout(new FillLayout());
		
		emoticonsScroll = new ScrolledComposite(group, SWT.V_SCROLL);
		emoticonsComposite = new Composite(emoticonsScroll, SWT.NONE);
		emoticonsComposite.setLayout(new GridLayout(4, false));
		
		createBoldLabel(emoticonsComposite, "Protocol:", gridData(2, 1));
		createProtoCombo(emoticonsComposite, protocol, main, group);
		
		for (final Emoticon emo : mep.emoticons)
		{
			EmoticonImage icon = emo.icons.get(protocol);
			if (icon == null)
				continue;
			
			createEmoticonNameLabel(emoticonsComposite, icon);
			
			createIconCombo(emoticonsComposite, icon);
			createFramesHolder(emoticonsComposite, icon);
		}
		
		emoticonsScroll.setContent(emoticonsComposite);
		emoticonsScroll.setExpandHorizontal(true);
		emoticonsScroll.setExpandVertical(true);
		emoticonsScroll.addControlListener(new ControlAdapter() {
			@Override
			public void controlResized(ControlEvent e)
			{
				Rectangle r = emoticonsScroll.getClientArea();
				emoticonsScroll.setMinSize(emoticonsComposite.computeSize(r.width, SWT.DEFAULT));
			}
		});
		
//		emoticonsComposite.pack();
		emoticonsScroll.setMinSize(emoticonsComposite.computeSize(SWT.DEFAULT, SWT.DEFAULT, false));
	}
	
	private static void createEmoticonNameLabel(final Composite parent, EmoticonImage icon)
	{
		File defImage = new File("data/Defaults/" + icon.emoticon.name + ".png");
		if (defImage.exists())
		{
			Label label = new Label(parent, SWT.NONE);
			label.setLayoutData(gridData(1, 2));
			label.setImage(Images.get(defImage));
			
			createBoldLabel(parent, getEmoLabelText(icon), gridData(1, 2));
		}
		else
		{
			createBoldLabel(parent, getEmoLabelText(icon), gridData(2, 2));
		}
	}
	
	private static String getEmoLabelText(EmoticonImage icon)
	{
		if (icon.description == null)
			return shorten(icon.emoticon.name);
		if (icon.description.equalsIgnoreCase(icon.emoticon.name))
			return shorten(icon.description);
		
		return shorten(icon.description) + "\n[" + shorten(icon.emoticon.name) + "]";
	}
	
	private static String shorten(String str)
	{
		if (str.length() < 22)
			return str;
		return str.substring(0, 18) + " ...";
	}
	
	private static void createProtoCombo(final Composite parent, String protocol, final Composite main, final Group group)
	{
		protoCombo = new Combo(parent, SWT.READ_ONLY);
		protoCombo.setLayoutData(gridData(GridData.FILL_HORIZONTAL, 2, 1));
		protoCombo.add("All protocols");
		for (String p : protocols)
			protoCombo.add(p);
		protoCombo.select(protocol == null ? 0 : protoCombo.indexOf(protocol));
		protoCombo.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e)
			{
				runAllDelayedListeners();
				
				String proto = getCurrentSelectedProtocol();
				
				group.dispose();
				
				createEmoticonsGroup(main, proto);
				
				shell.layout();
			}
		});
	}
	
	private static GridData gridData(int style, int horizontalSpan, int verticalSpan)
	{
		GridData gd = new GridData(style);
		gd.horizontalSpan = horizontalSpan;
		gd.verticalSpan = verticalSpan;
		return gd;
	}
	
	private static GridData gridData(int horizontalSpan, int verticalSpan)
	{
		GridData gd = new GridData();
		gd.horizontalSpan = horizontalSpan;
		gd.verticalSpan = verticalSpan;
		return gd;
	}
	
	private static void createIconCombo(Composite parent, final EmoticonImage icon)
	{
		Label label = new Label(parent, SWT.NONE);
		label.setText("Icon:");
		
		final Combo combo = new Combo(parent, SWT.NONE);
		combo.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		
		if (icon.image != null)
			combo.setText(icon.image.path);
		
		combo.addListener(SWT.FocusIn, new Listener() {
			public void handleEvent(Event event)
			{
				if (combo.getItemCount() > 0)
					return;
				
				inGuiSetCommand = true;
				
				for (ImageFile file : mep.images)
					combo.add(file.path);
				
				inGuiSetCommand = false;
			}
		});
		
		DelayedListener listener = new DelayedListener(new Listener() {
			public void handleEvent(Event e)
			{
				if (combo.getText().trim().length() <= 0)
					icon.image = null;
				else
					icon.image = mep.createTemporaryImage(combo.getText(), icon.protocol);
				fillFramesHolder(icon, false);
			}
		});
		combo.addListener(SWT.Selection, listener.fast());
		combo.addListener(SWT.Modify, listener.delayed());
	}
	
	private static final Set<DelayedListener> delayedListeners = new HashSet<DelayedListener>();
	
	private static void runAllDelayedListeners()
	{
		while (delayedListeners.size() > 0)
		{
			delayedListeners.iterator().next().run();
		}
	}
	
	static class DelayedListener
	{
		private Event event;
		private Listener listener;
		private Color bkg;
		private Runnable run = new Runnable() {
			public void run()
			{
				DelayedListener.this.run();
			}
		};
		private Color color;
		
		public DelayedListener(Listener listener)
		{
			this.listener = listener;
		}
		
		private void run()
		{
			Display.getCurrent().timerExec(-1, run);
			delayedListeners.remove(this);
			((Control) event.widget).setBackground(bkg);
			listener.handleEvent(event);
			if (color != null)
			{
				color.dispose();
				color = null;
			}
		}
		
		private void start(Event e)
		{
			event = e;
			
			if (bkg == null)
				bkg = ((Control) e.widget).getBackground();
			color = new Color(Display.getCurrent(), 255, 255, 200);
			((Control) e.widget).setBackground(color);
			
			delayedListeners.add(this);
			Display.getCurrent().timerExec(-1, run);
			Display.getCurrent().timerExec(3000, run);
		}
		
		public Listener delayed()
		{
			return new Listener() {
				public void handleEvent(Event e)
				{
					if (inGuiSetCommand)
						return;
					
					start(e);
				}
			};
		}
		
		public Listener fast()
		{
			return new Listener() {
				public void handleEvent(Event e)
				{
					if (inGuiSetCommand)
						return;
					
					event = e;
					run();
				}
			};
		}
	};
	
	private static void fillFramesHolder(final EmoticonImage icon, boolean creating)
	{
		if (icon.image != null && icon.image.frames == null && (!creating || icon.image.haveToDownload()))
		{
			Display.getCurrent().asyncExec(new Runnable() {
				public void run()
				{
					if (shell.isDisposed())
						return;
					
					createFrames(icon, true);
				}
			});
		}
		else
		{
			createFrames(icon, !creating);
		}
	}
	
	private static void createFrames(final EmoticonImage icon, boolean layout)
	{
		if (!equals(icon.protocol, getCurrentSelectedProtocol()))
			return;
		
		if (icon.emoticon.frames.isDisposed())
			return;
		
		boolean disposed = disposeAllChildren(icon.emoticon.frames);
		
		final ImageFile image;
		if (icon.image == null)
			image = icon.emoticon.icons.get(null).image;
		else
			image = icon.image;
		
		if (image == null)
		{
			if (disposed && layout)
				layout(icon.emoticon);
			return;
		}
		
		image.loadFrames();
		if (image.frames == null)
		{
			if (disposed && layout)
				layout(icon.emoticon);
			return;
		}
		
		Listener listener = new Listener() {
			public void handleEvent(Event e)
			{
				Button button = (Button) e.widget;
				
				Control[] children = icon.emoticon.frames.getChildren();
				for (int i = 0; i < children.length; i++)
				{
					Control child = children[i];
					if (e.widget != child && child instanceof Button && (child.getStyle() & SWT.TOGGLE) != 0)
					{
						((Button) child).setSelection(false);
					}
					else if (child == button)
					{
						image.frame = i;
					}
				}
				button.setSelection(true);
			}
		};
		
		Image[] frames = image.frames;
		for (int j = 0; j < frames.length; j++)
		{
			Button button = new Button(icon.emoticon.frames, SWT.TOGGLE);
			button.setImage(frames[j]);
			if (j == image.frame)
				button.setSelection(true);
			button.addListener(SWT.Selection, listener);
		}
		
		if (layout)
			layout(icon.emoticon);
	}
	
	private static void layout(Emoticon emo)
	{
		emo.frames.layout();
		Rectangle r = emoticonsScroll.getClientArea();
		emoticonsScroll.setMinSize(emoticonsComposite.computeSize(r.width, SWT.DEFAULT));
	}
	
	private static boolean disposeAllChildren(final Composite composite)
	{
		Control[] children = composite.getChildren();
		for (int i = 0; i < children.length; i++)
			children[i].dispose();
		return children.length > 0;
	}
	
	private static void createFramesHolder(Composite parent, EmoticonImage icon)
	{
		Label label = new Label(parent, SWT.NONE);
		label.setText("Frame:");
		
		icon.emoticon.frames = new Composite(parent, SWT.NONE);
		icon.emoticon.frames.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		RowLayout rowLayout = new RowLayout(SWT.HORIZONTAL);
		rowLayout.wrap = true;
		icon.emoticon.frames.setLayout(rowLayout);
		
		fillFramesHolder(icon, true);
	}
	
	private static Label createBoldLabel(Composite parent, String text)
	{
		Label label = new Label(parent, SWT.NONE);
		label.setLayoutData(new GridData());
		label.setText(text);
		setBoldFont(label);
		return label;
	}
	
	private static Label createBoldLabel(Composite parent, String text, GridData gd)
	{
		Label label = new Label(parent, SWT.NONE);
		label.setLayoutData(gd);
		label.setText(text);
		setBoldFont(label);
		return label;
	}
	
	private static Font boldFont;
	
	private static void setBoldFont(Label name)
	{
		if (boldFont == null)
		{
			Font initialFont = name.getFont();
			FontData[] fontData = initialFont.getFontData();
			for (int i = 0; i < fontData.length; i++)
				fontData[i].setStyle(SWT.BOLD);
			boldFont = new Font(Display.getCurrent(), fontData);
		}
		name.setFont(boldFont);
	}
	
	private static String getCurrentSelectedProtocol()
	{
		String proto = protoCombo.getText();
		if (!protocols.contains(proto))
			proto = null;
		return proto;
	}
	
	private static boolean equals(Object a, Object b)
	{
		if (a == null && b == null)
			return true;
		if (a == null || b == null)
			return true;
		return a.equals(b);
	}
}
