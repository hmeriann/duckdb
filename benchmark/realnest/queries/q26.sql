SELECT 
  list_aggregate(tau_pt, 'product'),
  list_aggregate(tau_eta, 'product'),
  list_aggregate(jet_pt, 'product'),
  list_aggregate(jet_eta, 'product'),
  list_aggregate(muon_pt, 'product'),
  list_aggregate(muon_eta, 'product'),
  list_aggregate(el_pt, 'product'),
  list_aggregate(el_eta, 'product'),
  list_aggregate(ph_pt, 'product'),
  list_aggregate(ph_eta, 'product')  
FROM singleMu;