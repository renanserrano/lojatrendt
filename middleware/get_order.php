<?
	header('Content-type: text/plain');
	session_start();

	require __DIR__.'/vendor/autoload.php';
	use phpish\shopify;

	require __DIR__.'/conf.php';

	$shopify = shopify\client(SHOPIFY_SHOP, SHOPIFY_APP_API_KEY, SHOPIFY_APP_PASSWORD, true);

	try{
		$shop = $shopify('GET /admin/orders.json?name=' . $_GET['order']);
		if($shop[0]['financial_status'] == "paid" && $shop[0]['email'] == $_GET['email']){
			print 1;
		}else{
			print 0;
		}
	}catch (shopify\ApiException $e){
		echo $e;
		print_R($e->getRequest());
		print_R($e->getResponse());
	}catch (shopify\CurlException $e){
		# cURL error
		echo $e;
		print_R($e->getRequest());
		print_R($e->getResponse());
	}

?>