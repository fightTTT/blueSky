using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Text.RegularExpressions;//正規表現用
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;//プロジェクトシリアライズ用

namespace ActionTool
{
    public partial class frmImage : Form
    {
		//定数
		const uint MAX_SCALE = 800;
		const uint MIN_SCALE = 50;

		//オリジナル画像系
		string _originalFileName;
		Image _originalImage;//元画像保持用
		uint _originalImageScale = 100;//元画像表示比率(%)
		Point _cutBeginPos;//カット開始位置
		Bitmap _filterLayerBmp;//フィルターレイヤービットマップオブジェクト
		Rectangle _cutRect;//切り取り矩形
		Rectangle _actualCutRect;//切り取り矩形の実寸のサイズ

		//中心点画像系
		uint _centeredImageScale = 100;//中心点画像倍率
		Point _centerPos;//中心点位置 
		Image _centeredImage;//中心点画像保持用
		Bitmap _centeredFilterLayerBMP;//中心点系フィルターレイヤー用

		//プロジェクト用
		/// <summary>
		/// 切り抜き情報
		/// </summary>
		[DataContract]
		struct CuttingInfo
		{
			[DataMember(Name = "cutrect")]
			public Rectangle cutRect;//切り抜き矩形
			[DataMember(Name = "center")]
			public Point center;//中心点
			[DataMember(Name = "duration")]
			public int duration;//継続時間
		}
		//切り抜き情報テーブル
		Dictionary<string, List<CuttingInfo>> _cuttingTable=new Dictionary<string, List<CuttingInfo>>();

		/// <summary>
		/// プロジェクトとして保存する用
		/// </summary>
		[DataContract]
		class ProjectInfo
		{
			[DataMember(Name = "filename")]
			public string originalFileName { get; set; }//元ファイル名(複数に対応させるかも)
			[DataMember(Name = "cuttable")]
			public Dictionary<string, List<CuttingInfo>> cuttingTable { get; set; }//切り抜き情報テーブル
		}

		string _savedFileName = "";

        public frmImage()
        {
            InitializeComponent();
        }

        private void frmImage_Load(object sender, EventArgs e)
		{
			//オリジナル画像部分初期化
			InitializeForOriginalImage();
			//中心点設定部分初期化
			InitializeForCenteredImage();
		}

		/// <summary>
		/// 中心点設定部分初期化
		/// </summary>
		private void InitializeForCenteredImage()
		{
			pictCenteredFilter.Parent = pictCutImage;
			pictCenteredFilter.BackColor = Color.Transparent;
			pictCenteredFilter.Cursor = Cursors.Cross;
			//切り抜きの矩形とかを描画する用のレイヤー作成
			_centeredFilterLayerBMP = new Bitmap(pictCenteredFilter.Width, pictCenteredFilter.Height);
			Graphics g = Graphics.FromImage(_centeredFilterLayerBMP);
			//透明色で塗りつぶし
			g.FillRectangle(Brushes.White, 0, 0, _centeredFilterLayerBMP.Width, _centeredFilterLayerBMP.Height);
			_centeredFilterLayerBMP.MakeTransparent(Color.White);
			pictCenteredFilter.Image = _centeredFilterLayerBMP;
			g.Dispose();
		}

		/// <summary>
		/// オリジナル画像部分初期化
		/// </summary>
		private void InitializeForOriginalImage()
		{
			//切り抜きの矩形とかを描画する用のレイヤー作成
			CreatePictureAndImageForCutRect();

			//背面に市松模様を表示させるためのpictOriginalBackその他設定
			pictOriginal.Parent = panelOriginal;
			panelOriginal.Parent = pictOriginalBack;
			pictOriginalBack.Width = panelOriginal.Width;
			pictOriginalBack.Height = panelOriginal.Height;

			//市松模様描画
			CreateBackestForOriginalImage();
			((Control)pictOriginalFilter).AllowDrop = true;
		}

		/// <summary>
		/// 市松模様描画
		/// </summary>
		private void CreateBackestForOriginalImage()
		{
			HatchBrush hb = new HatchBrush(HatchStyle.LargeCheckerBoard, Color.LightGray, Color.White);
			Bitmap checker = new Bitmap(pictOriginalBack.Width, pictOriginalBack.Height);
			Graphics g = Graphics.FromImage(checker);
			g.FillRectangle(hb, 0, 0, pictOriginalBack.Width, pictOriginalBack.Height);
			pictOriginalBack.Image = checker;
			g.Dispose();
		}

		/// <summary>
		/// 切り抜きの矩形とかを描画する用のレイヤー作成
		/// </summary>
		private void CreatePictureAndImageForCutRect()
        {
            //切り抜きの矩形とかを描画する用のレイヤー作成
            _filterLayerBmp = new Bitmap(pictOriginalFilter.Width, pictOriginalFilter.Height);
            Graphics g = Graphics.FromImage(_filterLayerBmp);
            //透明色で塗りつぶし
            g.FillRectangle(Brushes.White, 0, 0, _filterLayerBmp.Width, _filterLayerBmp.Height);
            _filterLayerBmp.MakeTransparent(Color.White);
            pictOriginalFilter.Image = _filterLayerBmp;
            pictOriginalFilter.Parent = pictOriginal;
			g.Dispose();
        }


		/// <summary>
		/// 元の画像から、拡大縮小したイメージを返す
		/// </summary>
		/// <param name="scale">拡大率</param>
		/// <returns>拡大縮小済みのイメージ</returns>
		private Image GetScaledImageFromImage(Image inImage,float scale)
		{
			Bitmap canvas = new Bitmap((int)(inImage.Width * scale), (int)(inImage.Height * scale));
			Graphics g = Graphics.FromImage(canvas);
			g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
			g.DrawImage(inImage, 0, 0, inImage.Width * scale, inImage.Height * scale);
			g.Dispose();
			return canvas;
		}

		/// <summary>
		/// オリジナル画像表示部分のリフレッシュ
		/// </summary>
		private void RefreshOriginalImage()
		{
			if (_originalImage == null) return;
			lblScale.Text = "倍率=" + _originalImageScale.ToString() + "%";
			float scale = (float)_originalImageScale / 100.0f;
			pictOriginal.Image.Dispose();
			pictOriginal.Image = GetScaledImageFromImage(_originalImage, scale);
		}

		/// <summary>
		/// オリジナル画像部分の拡大
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnZoomIn_Click(object sender, EventArgs e)
		{
			if (pictOriginal.Image == null) return;
			_originalImageScale = Math.Min(_originalImageScale + 50, MAX_SCALE);
			RefreshOriginalImage();


			pictOriginalFilter.Image.Dispose();
			Bitmap canvas = new Bitmap(pictOriginal.Image.Width, pictOriginal.Image.Height);

			Graphics g = Graphics.FromImage(canvas);
			g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
			g.Clear(Color.Transparent);
			g.Dispose();
			pictOriginalFilter.Image = canvas;
			//スケーリングを考慮した、オリジナル画像への矩形描画
			DrawCutRectToOriginalFilterLayerWithScale();

		}

		/// <summary>
		/// スケーリングを考慮した、オリジナル画像への矩形描画
		/// </summary>
		/// <param name="scale"></param>
		private void DrawCutRectToOriginalFilterLayerWithScale( )
		{
		if (_actualCutRect == null) return;
			
			float scale = (float)_originalImageScale / 100.0f;
			Rectangle rc = _actualCutRect;
			rc.X = (int)(rc.X * scale);
			rc.Y = (int)(rc.Y * scale);
			rc.Width = (int)(rc.Width * scale);
			rc.Height = (int)(rc.Height * scale);
			DrawRectToOriginalFilterLayer(rc);
		}

		/// <summary>
		/// オリジナル画像部分の縮小
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnZoomOut_Click(object sender, EventArgs e)
		{
			//スケール値変更
			_originalImageScale = Math.Max(_originalImageScale - 50, MIN_SCALE);

			//元画像の拡大表示
			RefreshOriginalImage();

			//スケーリングを考慮した、オリジナル画像への矩形描画
			DrawCutRectToOriginalFilterLayerWithScale();

		}

		/// <summary>
		/// オリジナル画像表示サイズリセット(100%に)
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnReset_Click(object sender, EventArgs e)
		{
			_originalImageScale = 100;
			RefreshOriginalImage();

			//スケーリングを考慮した、オリジナル画像への矩形描画
			DrawCutRectToOriginalFilterLayerWithScale();
		}
		
		/// <summary>
		/// オリジナル画像のドラッグアンドドロップ
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void panelOriginal_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				string[] filename = (string[])(e.Data.GetData(DataFormats.FileDrop));
				LoadOriginalPicture(filename[0]);
			}
		}

		/// <summary>
		/// 元画像のロード
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		/// <returns>ロード成功/失敗</returns>
		private bool LoadOriginalPicture(string fileName)
		{
			if (!System.IO.File.Exists(fileName)) {
				return false;
			}
			_originalImage = Image.FromFile(fileName);
			pictOriginal.Image = (Image)_originalImage.Clone();
			_originalFileName = fileName;
			return true;
		}

		/// <summary>
		/// オリジナル画像へのドラッグの際にアイコンを書き換えてるだけ
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void panelOriginal_DragEnter(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				e.Effect = DragDropEffects.Copy;
			} else {
				e.Effect = DragDropEffects.None;
			}
		}

		/// <summary>
		/// オリジナル画像上で現在のカーソル位置を記録
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void pictOriginalFilter_MouseDown(object sender, MouseEventArgs e)
		{
			_cutBeginPos.X = Math.Max(e.Location.X,0);
			_cutBeginPos.Y = Math.Max(e.Location.Y, 0);
		}

		/// <summary>
		/// 実際の切り取り矩形サイズを返す
		/// </summary>
		/// <returns>拡大縮小を考慮に入れない矩形情報</returns>
		private Rectangle GetActualCutRect()
		{
			float scale = (float)_originalImageScale / 100.0f;
			Rectangle ret = new Rectangle();
			ret.X = (int)(_cutRect.X / scale);
			ret.Y = (int)(_cutRect.Y / scale);
			ret.Width = (int)(_cutRect.Width / scale);
			ret.Height = (int)(_cutRect.Height / scale);
			return ret;
		}



		/// <summary>
		/// オリジナル画像上でマウスを動かしたときの挙動
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void pictOriginalFilter_MouseMove(object sender, MouseEventArgs e)
		{
			//現在位置をラベルに表示
			Point p = e.Location;
			float scale = (float)_originalImageScale / 100.0f;
			p.X = Math.Max((int)(p.X / scale), 0);
			p.Y = Math.Max((int)(p.Y / scale), 0);
			lblPosition.Text = p.ToString();

			//ドラッグ中
			if (e.Button == MouseButtons.Left)
			{
				//ドラッグ中の矩形の作成と描画
				DrawDraggingRectForOriginal(e.Location);

				lblCutRect.Text = "切抜矩形=" + GetActualCutRect().ToString();
			}
		}

		/// <summary>
		/// ドラッグ中の矩形の作成と描画
		/// </summary>
		/// <param name="pos"></param>
		private void DrawDraggingRectForOriginal(Point pos)
		{
			Rectangle rc = CreateRectFor2Point(pos, _cutBeginPos);
			DrawRectToOriginalFilterLayer(rc);
			_cutRect = rc;
		}

		/// <summary>
		/// 元画像レイヤーに矩形を描画
		/// </summary>
		/// <param name="rc"></param>
		private void DrawRectToOriginalFilterLayer(Rectangle rc)
		{
			Graphics g = Graphics.FromImage(pictOriginalFilter.Image);
			g.Clear(Color.Transparent);
			Pen pen = new Pen(Color.White, 2);
			g.DrawRectangle(pen, rc);

			pen.Color = Color.Black;
			pen.DashStyle = DashStyle.Dot;
			g.DrawRectangle(pen, rc);

			pictOriginalFilter.Refresh();
			pen.Dispose();
			g.Dispose();
		}

		private static Rectangle CreateRectFor2Point(Point  pos1, Point pos2)
		{
			//ドラッグにより形成される矩形を計算
			int px = Math.Max(0, pos1.X);
			int py = Math.Max(0, pos1.Y);
			int x = Math.Min(pos2.X, px);
			int y = Math.Min(pos2.Y, py);
			int w = Math.Abs(pos2.X - px);
			int h = Math.Abs(pos2.Y - py);
			if (w == 0 || h == 0) return new Rectangle();
			return new Rectangle(x, y, w, h);

		}


		private void pictOriginalFilter_MouseUp(object sender, MouseEventArgs e)
		{
			_cutRect=CreateRectFor2Point(e.Location, _cutBeginPos);
			_actualCutRect = GetActualCutRect();
			lblCutRect.Text = "切抜矩形=" + GetActualCutRect().ToString();
		}

		private void panelOriginal_Scroll(object sender, ScrollEventArgs e)
		{
			if (e.ScrollOrientation == ScrollOrientation.VerticalScroll) {
				pictOriginal.Location=new Point(pictOriginal.Location.X,-e.NewValue);
			} else {
				pictOriginal.Location = new Point(-e.NewValue,pictOriginal.Location.Y);
			}
		}

		private void pictOriginal_Click(object sender, EventArgs e)
		{
			
		}

		private void panelOriginal_Click(object sender, EventArgs e)
		{
			
		}

		private void frmImage_KeyDown(object sender, KeyEventArgs e)
		{

		}
		/// <summary>
		/// 現在の矩形でオリジナル画像のカットを行い、DBに登録する
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnCut_Click(object sender, EventArgs e)
		{
			Rectangle rect = _actualCutRect;
			DisplayCutPicture(rect, new Point(rect.Width / 2, rect.Height));
			SetDefaultActionName();
		}

		/// <summary>
		/// 元画像を矩形で切り抜いた
		/// </summary>
		/// <param name="rect"></param>
		private void DisplayCutPicture(Rectangle rect,Point p)
		{
			if (rect.Width == 0 || rect.Height == 0) {
				return;
			}
			Bitmap bmp = new Bitmap(rect.Width, rect.Height);
			Graphics g = Graphics.FromImage(bmp);
			Rectangle drect = rect;
			drect.X = 0;
			drect.Y = 0;
			g.DrawImage(_originalImage, drect, rect, GraphicsUnit.Pixel);
			_centeredImage = bmp;
			pictCutImage.Image = bmp;
			pictCutImage.Refresh();
			DrawCrossLineToCenterLayer(p);
		}

		/// <summary>
		/// デフォルトアクション名設定
		/// </summary>
		private void SetDefaultActionName()
		{
			if (txtActionName.Text == "" || Regex.IsMatch(txtActionName.Text, @"アクション\d\d\d")) {
				var items = from item in listActions.Items.Cast<string>()
							orderby item descending
							where item.StartsWith("アクション")
							select item;
				foreach (var actioname in items) {
					if (Regex.IsMatch(actioname, @"アクション\d\d\d")) {
						var strnumber = actioname.Substring("アクション".Length);
						int number = 0;
						if (Int32.TryParse(strnumber, out number)) {
							txtActionName.Text = String.Format("アクション{0:D3}", number + 1);
							return;
						}
					}
				}
				txtActionName.Text = "アクション000";
			}
		}

		private void pictCutImage_Click(object sender, EventArgs e)
        {
            //Todo:中心点設定機能を追加
        }

        private void btnFixCenter_Click(object sender, EventArgs e)
        {
			CuttingInfo ct;
			ct.cutRect = _cutRect;
			ct.center = _centerPos;
			ct.duration = (int)txtDuration.Value;
			List<CuttingInfo> ctlist;
			if (listActions.Items.Contains(txtActionName.Text)) {
				_cuttingTable[txtActionName.Text].Add(ct);
				AddCutPictureToImageList();
			} else {
				listActions.Items.Add(txtActionName.Text);
				ctlist = new List<CuttingInfo>();
				ctlist.Add(ct);
				_cuttingTable.Add(txtActionName.Text, ctlist);
				AddCutPictureToImageList();

			}
        }

		/// <summary>
		/// イメージリストに現在の切り抜き画像を登録する
		/// </summary>
		private void AddCutPictureToImageList()
		{
			Image cutimg = pictCutImage.Image;
			Image img = new Bitmap(40, 40);
			Rectangle destRc = new Rectangle(0, 0, img.Width, img.Height);
			Graphics g = Graphics.FromImage(img);
			g.DrawImage(pictCutImage.Image, destRc, new Rectangle(0, 0, cutimg.Width, cutimg.Height), GraphicsUnit.Pixel);
			string key = imgList.Images.Count.ToString();
			imgList.Images.Add(key, img);
			listPictures.Items.Add(txtActionName.Text + ":" + key, key);
			g.Dispose();
		}

		/// <summary>
		/// 中心点画像表示部分のリフレッシュ
		/// </summary>
		private void RefreshCenteredImage()
		{
			//if (_originalImage == null) return;
			//lblScale.Text = "倍率=" + _originalImageScale.ToString() + "%";

			//スケーリング済み切り抜き画像の表示
			float scale = (float)_centeredImageScale / 100.0f;
			//pictCutImage.Image.Dispose();
			pictCutImage.Image = GetScaledImageFromImage(_centeredImage, scale);
			pictCutImage.Refresh();

			lblCenterPos.Text = _centerPos.ToString();

			//スケーリング済み中心点を表示
			Point p = _centerPos;
			p.X = (int)(p.X*scale);
			p.Y = (int)(p.Y*scale);
			DrawCrossLineToCenterLayer(p);
		}
		private void btnCenterExpand_Click(object sender, EventArgs e)
        {
			if (_centeredImage == null) return;
			_centeredImageScale = Math.Min(_centeredImageScale + 50, MAX_SCALE);
			lblCenterScale.Text = "倍率=" + _centeredImageScale.ToString() + "%";
			RefreshCenteredImage();
		}

        private void btnCenterShrink_Click(object sender, EventArgs e)
        {
			if (_centeredImage == null) return;
			_centeredImageScale = Math.Max(_centeredImageScale - 50, MIN_SCALE);
			lblCenterScale.Text = "倍率=" + _centeredImageScale.ToString() + "%";
			RefreshCenteredImage();
		}

		private void btnCenterReset_Click(object sender, EventArgs e)
        {
			if (_centeredImage == null) return;
			_centeredImageScale = 100;
			lblCenterScale.Text = "倍率=" + _centeredImageScale.ToString() + "%";
			RefreshCenteredImage();
		}

		private void pictCenteredFilter_MouseMove(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				//現在中心位置をラベルに表示
				Point p = e.Location;

				//最前面レイヤーに十字を描画
				DrawCrossLineToCenterLayer(p);

				//実際の中心点を保持
				float scale = (float)_centeredImageScale / 100.0f;
				p.X = Math.Max((int)(p.X / scale), 0);
				p.Y = Math.Max((int)(p.Y / scale), 0);
				_centerPos = p;
				lblCenterPos.Text = _centerPos.ToString();
			}
		}

		/// <summary>
		/// 中心点最前面レイヤーに十字を描画
		/// </summary>
		/// <param name="p"></param>
		private void DrawCrossLineToCenterLayer(Point p)
		{
			Graphics g = Graphics.FromImage(pictCenteredFilter.Image);
			g.Clear(Color.Transparent);
			Pen pen = new Pen(Color.White, 2);
			g.DrawLine(pen, new Point(p.X, 0), new Point(p.X, pictCenteredFilter.Height));
			g.DrawLine(pen, new Point(0, p.Y), new Point(pictCenteredFilter.Width, p.Y));

			pen.Color = Color.Black;
			pen.DashStyle = DashStyle.Dot;
			g.DrawLine(pen, new Point(p.X, 0), new Point(p.X, pictCenteredFilter.Height));
			g.DrawLine(pen, new Point(0, p.Y), new Point(pictCenteredFilter.Width, p.Y));

			pen.Dispose();
			g.Dispose();
			pictCenteredFilter.Refresh();
		}

		private void pictCenteredFilter_MouseEnter(object sender, EventArgs e)
		{
			
		}

		private void mnuSaveProject_Click(object sender, EventArgs e)
		{
			if (_savedFileName == "") {
				if (saveProjectFile.ShowDialog() == DialogResult.Cancel) {
					return;
				}
			}
			_savedFileName = saveProjectFile.FileName.ToString();
			SaveNamedProjectFile(_savedFileName);
		}

		private void SaveNamedProjectFile(string savefileName)
		{
			var proj = new ProjectInfo();
			proj.originalFileName = _originalFileName;
			proj.cuttingTable = _cuttingTable;
			_savedFileName = savefileName;
			//シリアライズ準備
			var fs = new System.IO.FileStream(savefileName, System.IO.FileMode.Create);//
			
			//シリアライザーにProjectInfoの内容を書き込み
			var serializer = new DataContractJsonSerializer(typeof(ProjectInfo));
			serializer.WriteObject(fs, proj);
			fs.Close();
		}

		private void openProject_Click(object sender, EventArgs e)
        {
            if(openProjectFile.ShowDialog()== DialogResult.Cancel){
                return;
            }
			//保存したJSONをロードして、保存時の状態を復元する
			var serializer = new DataContractJsonSerializer(typeof(ProjectInfo));
			var fs = new System.IO.FileStream(openProjectFile.FileName.ToString(), System.IO.FileMode.Open);//
			ProjectInfo  proj=(ProjectInfo)serializer.ReadObject(fs);
			_originalFileName=proj.originalFileName;
			_cuttingTable=proj.cuttingTable;
			_originalImage= Image.FromFile(_originalFileName);
			pictOriginal.Image = _originalImage;
			fs.Close();
			listActions.Items.Clear();
			foreach(var item in _cuttingTable) {
				listActions.Items.Add(item.Key);
			}
			_savedFileName = openProjectFile.FileName.ToString();
		}

		private void listCutRects_SelectedIndexChanged(object sender, EventArgs e)
		{
			var key=((ListBox)sender).SelectedItem.ToString();
			var value = _cuttingTable[key];
			DisplayCutPicture(value[0].cutRect, value[0].center);
		}

		private void mnuSaveNamedProjectFile_Click(object sender, EventArgs e)
		{
			//プロジェクトを名前をつけて保存
			if (saveProjectFile.ShowDialog() == DialogResult.Cancel) {
				return;
			}
			_savedFileName = saveProjectFile.FileName.ToString();
			SaveNamedProjectFile(_savedFileName);
		}

		private void btnLoadOriginal_Click(object sender, EventArgs e)
		{
			//オリジナル画像のロード
			if(openOriginalPicture.ShowDialog()== DialogResult.Cancel) {
				return;
			}
			LoadOriginalPicture(openOriginalPicture.FileName.ToString());
		}

		private void mnuWriteBinary_Click(object sender, EventArgs e)
		{
			//Todo:バイナリ出力実装
		}

		private void pictCenteredFilter_Click(object sender, EventArgs e)
		{

		}
	}
}
